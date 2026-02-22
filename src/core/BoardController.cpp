#include "BoardController.h"
#include "Board.h"
#include "src/ui/ui_Board.h"

#include <QGraphicsScene>
#include <QMessageBox>
#include <QRegularExpression>
#include <QAction>
#include <QPainter>
#include <QObject>
#include <QIcon>

#include <serializers/serializer.h>
#include "commandManager/add_node_command.h"
#include "commandManager/add_branch_command.h"
#include "commandManager/remove_branch_command.h"

BoardController::BoardController(Board* board, DiagramGraph* graph)
    : QObject(board)
    , window(board)
    , diagram(graph)
    , recovery_log(new recoveryLog(this))
    , mDialogFactory(new DialogFactory(board))
    , m_command_manager(new CommandManager(this))
    , mConnectionHandler(new ConnectionHandler(this))
    , menuBar(new QMenuBar(board))
{
    setParent(board);
    diagram->setParent(this);
    board->show();
    // Connect board items
    for(auto it = board->getNodes().begin(); it != board->getNodes().end(); it++) {
        connectNewNodeView(*it);
    }

    // Connect export json
    connect(board->ui->exportJSON, &QPushButton::clicked, this, &BoardController::requestJSONPath);
    connect(&Serializer::instance(), &Serializer::serializationSuccess, this, &BoardController::showSerializationSuccessMessage);
    connect(&Serializer::instance(), &Serializer::serializationFailure, mDialogFactory, &DialogFactory::showError);

    // Connect export png
    connect(board->ui->exportPNG, &QPushButton::clicked, this, &BoardController::requestPNGPath);

    // Connect generate project
    connect(board->ui->generate_project, &QPushButton::clicked, mDialogFactory->generateDialog, &QDialog::exec);
    connect(mDialogFactory, &DialogFactory::projectGenerationRequested, this, &BoardController::generateProject);

    // Connect radio buttons to command handler
    connect(window, &Board::radioButtonChecked, mConnectionHandler, &ConnectionHandler::updateSelectedBranchType);

    // Connect mode selection to command handler
    connect(window, &Board::modeChanged, mConnectionHandler, &ConnectionHandler::updateSelectedNodeModification);

    // Connect node creation
    connect(window, &Board::nodeCreationRequest, mDialogFactory, &DialogFactory::openNodeFactory);
    connect(mDialogFactory, &DialogFactory::createNodeRequest, diagram, &DiagramGraph::processNewNodeRequest);
    connect(diagram, &DiagramGraph::addNodeRequestApproved, m_command_manager, &CommandManager::addCreateNodeCommand);
    connect(m_command_manager, &CommandManager::createNodeCommandAdded, this, &BoardController::connectCreateNodeCommand);
    connect(diagram, &DiagramGraph::node_added, window, &Board::addItem);
    connect(window, &Board::itemCreated, this, &BoardController::connectNewNodeView);

    // Connect node removal
    connect(mConnectionHandler, &ConnectionHandler::nodeRemovalRequested, diagram, &DiagramGraph::processRemoveNodeRequest);
    connect(diagram, &DiagramGraph::removeNodeRequestApproved, m_command_manager, &CommandManager::addRemoveNodeCommand);
    connect(m_command_manager, &CommandManager::removeNodeCommandAdded, this, &BoardController::connectRemoveNodeCommand);
    connect(diagram, &DiagramGraph::node_removed, window, &Board::removeItem);
    connect(diagram, &DiagramGraph::node_removed, recovery_log, &recoveryLog::remove_node_operation);

    // Connect branch creation
    connect(mConnectionHandler, &ConnectionHandler::branchCreationRequested, diagram, &DiagramGraph::processNewBranchRequest);
    connect(diagram, &DiagramGraph::addBranchRequestApproved, m_command_manager, &CommandManager::addCreateBranchCommand);
    connect(m_command_manager, &CommandManager::createBranchCommandAdded, this, &BoardController::connectCreateBranchCommand);
    connect(diagram, &DiagramGraph::link_added, window, &Board::addLink);
    connect(diagram, &DiagramGraph::link_added, recovery_log, &recoveryLog::add_link_operation);

    // Connect branch removal
    connect(mConnectionHandler, &ConnectionHandler::branchRemovalRequested, diagram, &DiagramGraph::processRemoveBranchRequest);
    connect(diagram, &DiagramGraph::removeBranchRequestApproved, m_command_manager, &CommandManager::addRemoveBranchCommand);
    connect(m_command_manager, &CommandManager::removeBranchCommandAdded, this, &BoardController::connectRemoveBranchCommand);
    connect(diagram, &DiagramGraph::link_removed, window, &Board::removeLink);
    connect(diagram, &DiagramGraph::link_removed, recovery_log, &recoveryLog::remove_link_operation);

    // Show messages
    connect(diagram, &DiagramGraph::error, mDialogFactory, &DialogFactory::showError);
    connect(this, &BoardController::message, mDialogFactory, &DialogFactory::showMessage);
    connect(this, &BoardController::error, mDialogFactory, &DialogFactory::showError);
    connect(this, &BoardController::warning, mDialogFactory, &DialogFactory::showWarning);

    // Close window -> delete window
    connect(window, &QObject::destroyed, this, &QObject::deleteLater);

    // Connect node selection
    connect(mConnectionHandler, &ConnectionHandler::selectionRequested, this, &BoardController::select);
    connect(mConnectionHandler, &ConnectionHandler::deselectionRequested, this, &BoardController::deselect);

    setupActions();
}

BoardController::BoardController(const QString& title)
    : BoardController(new Board(title), new DiagramGraph()) {}

BoardController::~BoardController()
{
    qDebug() << "Deleted controller";
}

void BoardController::setupActions(){
    addMenu("File", {
        addAction("New project", QKeySequence::New, this, [](){}),
        addAction("Import project", QKeySequence::Open, this, [](){})
    });
    addMenuList("Edit", {
        {
        addAction("Undo", QKeySequence::Undo, m_command_manager, &CommandManager::undo, QIcon::fromTheme(QIcon::ThemeIcon::EditUndo)),
        addAction("Redo", QKeySequence::Redo, m_command_manager, &CommandManager::redo, QIcon::fromTheme(QIcon::ThemeIcon::EditRedo)),
        }, {
        addCtrlAltAction("New class", Qt::Key_C, window, &Board::onClassClicked),
        addCtrlAltAction("New struct", Qt::Key_S, window, &Board::onClassClicked)
        }
    });

    addCtrlAltAction("Linkage mode", Qt::Key_L, window->ui->linkageMode, &QCheckBox::click);
    addCtrlAltAction("Remove mode", Qt::Key_R, window->ui->removeMode, &QCheckBox::click);

    addAction("Next branch type", Qt::Key_Down, window, &Board::selectNextBranchType);
    addAction("Previous branch type", Qt::Key_Up, window, &Board::selectPreviousBranchType);

    window->layout()->setMenuBar(menuBar);
}

void BoardController::exportPNG(const QString& filePath){
    QGraphicsScene* scene = window->ui->board->scene();
    QRectF items_rect = scene->itemsBoundingRect();
    if(items_rect.isEmpty()){
        QMessageBox::warning(window, "Export", "No items to export!");
        return;
    }

    // margins [item] -> [  item  ]
    const qreal margin = 50;
    items_rect.adjust(-margin, -margin, margin, margin);

    QImage image(items_rect.size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    scene->render(&painter, QRectF(0, 0, image.width(), image.height()),  items_rect);
    painter.end();

    if(image.save(filePath, "PNG", 100)){
        emit message("Success", QString("Diagram exported to:\n%1\n\nSize: %2x%3 pixels")
                                        .arg(filePath).arg(image.width()).arg(image.height()));
    } else{
        emit error("Failed to save image. Check write permissions.");
    }
}

void BoardController::requestJSONPath()
{
    QString path = mDialogFactory->selectJSON(getFileName());
    Serializer::instance().serialize(path, window, diagram);
}

void BoardController::requestPNGPath()
{
    QString path = mDialogFactory->selectPNG(getFileName());
    exportPNG(path);
}

void BoardController::showSerializationSuccessMessage(const QString& filePath)
{
    emit message("Success", "Diagram exported to:\n" + filePath);
}

void BoardController::generateProject(const QString& path,
                         ProjectGenerator::FileNameNotation notation,
                         ProjectGenerator::ReplaceToggle toggle,
                         QString& projectDirName)
{
    ProjectGenerator::GenerationStatusCode status = ProjectGenerator::instance().generate(
        diagram->get_diagram(),
        path,
        projectDirName,
        notation,
        toggle
        );

    switch (status)
    {
    case ProjectGenerator::GenerationStatusCode::EXISTING_PROJECT_DIR_ON_PATH:
        emit warning("Directory with a given name already exists on path. Choose a different name or replace it.");
        break;
    case ProjectGenerator::GenerationStatusCode::PERMISSION_DENIED:
        emit warning("Permission denied.");
        break;
    case ProjectGenerator::GenerationStatusCode::NO_MEMORY_SPACE:
        emit warning("No enough memory on the disc.");
        break;
    case ProjectGenerator::GenerationStatusCode::FILE_NOT_CREATED:
        emit warning("Error while generating file.");
        break;
    case ProjectGenerator::GenerationStatusCode::NO_SUCH_DIR:
        emit warning("The selected path does not exist. Please choose an existing directory.");
        break;
    case ProjectGenerator::GenerationStatusCode::OK:
        emit message("Success", "Project is generated on path: \n" + path);
        break;
    }
}

QString BoardController::getFileName() {
    QString title = window->ui->title->text().trimmed();
    title = title.toLower();
    title.replace(QRegularExpression("\\s+"), "_");
    title.remove(QRegularExpression("^_+|_+$"));
    return title;
}

void BoardController::setSelect(NodePtr ptr, bool isSelected)
{
    auto node = diagram->find_pointer_owner(ptr);
    window->setSelected(node, isSelected);
}

void BoardController::connectCreateNodeCommand(std::shared_ptr<AddNodeCommand> command) {
    connect(command.get(), &AddNodeCommand::addNodeRequested, diagram, &DiagramGraph::addNode);
    connect(command.get(), &AddNodeCommand::removeNodeRequested, diagram, &DiagramGraph::removeNode);
}

void BoardController::connectRemoveNodeCommand(std::shared_ptr<RemoveNodeCommand> command)
{
    connect(command.get(), &RemoveNodeCommand::addNodeRequested, diagram, &DiagramGraph::addNode);
    connect(command.get(), &RemoveNodeCommand::removeNodeRequested, diagram, &DiagramGraph::removeNode);
    connect(command.get(), &RemoveNodeCommand::addBranchRequested, diagram, &DiagramGraph::addBranch);
    connect(command.get(), &RemoveNodeCommand::removeBranchRequested, diagram, &DiagramGraph::removeBranch);
}

void BoardController::connectCreateBranchCommand(std::shared_ptr<AddBranchCommand> command) {
    connect(command.get(), &AddBranchCommand::addBranchRequested, diagram, &DiagramGraph::addBranch);
    connect(command.get(), &AddBranchCommand::removeBranchRequested, diagram, &DiagramGraph::removeBranch);
}

void BoardController::connectRemoveBranchCommand(std::shared_ptr<RemoveBranchCommand> command) {
    connect(command.get(), &RemoveBranchCommand::addBranchRequested, diagram, &DiagramGraph::addBranch);
    connect(command.get(), &RemoveBranchCommand::removeBranchRequested, diagram, &DiagramGraph::removeBranch);
}

void BoardController::connectNewNodeView(CppClassView *item)
{
    connect(item, &CppClassView::objectClicked, mConnectionHandler, &ConnectionHandler::handleNodeClick);
    connect(item, &CppClassView::objectMoved, m_command_manager, &CommandManager::addMoveNodeCommand);
    connect(item, &CppClassView::add_field_request, recovery_log, &recoveryLog::add_field_operation);
    connect(item, &CppClassView::add_method_request, recovery_log, &recoveryLog::add_method_operation);
}

void BoardController::select(NodePtr nodePtr)
{
    setSelect(nodePtr, true);
}

void BoardController::deselect(NodePtr nodePtr)
{
    setSelect(nodePtr, false);
}

void BoardController::addMenuList(const QString &menuName, const QList<QList<QAction *>> &actions)
{
    auto menu = menuBar->addMenu("&" + menuName);

    for(auto it = actions.begin(); it != actions.end(); it++) {
        if(it != actions.begin()) {
            menu->addSeparator();
        }
        for(auto action : *it) {
            menu->addAction(action);
        }
    }
}

void BoardController::addMenu(const QString &menuName, const QList<QAction *> &actions)
{
    addMenuList(menuName, { actions });
}

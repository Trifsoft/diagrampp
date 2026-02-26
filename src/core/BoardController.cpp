#include "BoardController.h"
#include "Board.h"
#include "src/ui/ui_Board.h"

#include <QGraphicsScene>
#include <QRegularExpression>
#include <QAction>
#include <QObject>
#include <QIcon>
#include <model/base/branches.h>

#include <serializers/serializer.h>
#include "commandManager/add_node_command.h"
#include "commandManager/add_branch_command.h"

BoardController::BoardController(Board* board, DiagramGraph* graph)
    : QObject(board)
    , window(board)
    , diagram(graph)
    , recovery_log(new recoveryLog(this))
    , mDialogFactory(new DialogFactory(board))
    , m_command_manager(new CommandManager(this))
    //, mConnectionHandler(new ConnectionHandler(this))
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
    connect(&Serializer::instance(), &Serializer::serializationSuccess, this, &BoardController::showSerializationSuccessMessage);
    connect(&Serializer::instance(), &Serializer::serializationFailure, mDialogFactory, &DialogFactory::showError);

    // Connect generate project
    connect(mDialogFactory, &DialogFactory::projectGenerationRequested, this, &BoardController::generateProject);

    // Connect node creation
    connect(window, &Board::nodeCreationRequest, mDialogFactory, &DialogFactory::openNodeFactory);
    connect(mDialogFactory, &DialogFactory::createNodeRequest, diagram, &DiagramGraph::processNewNodeRequest);
    connect(diagram, &DiagramGraph::addNodeRequestApproved, m_command_manager, &CommandManager::addCreateNodeCommand);
    connect(m_command_manager, &CommandManager::createNodeCommandAdded, this, &BoardController::connectCreateNodeCommand);
    connect(diagram, &DiagramGraph::node_added, window, &Board::addItem);
    connect(window, &Board::itemCreated, this, &BoardController::connectNewNodeView);

    // Connect node removal
    //connect(window, &Board::nodeRemovalRequested, diagram, &DiagramGraph::processRemoveNodeRequest);
    //connect(diagram, &DiagramGraph::removeNodeRequestApproved, m_command_manager, &CommandManager::addRemoveNodeCommand);
    //connect(m_command_manager, &CommandManager::removeNodeCommandAdded, this, &BoardController::connectRemoveNodeCommand);
    connect(diagram, &DiagramGraph::node_removed, window, &Board::removeItem);
    connect(diagram, &DiagramGraph::node_removed, recovery_log, &recoveryLog::remove_node_operation);

    // Connect branch creation
    connect(window, &Board::branchMenuRequested, this, &BoardController::showBranchTypeMenu);
    connect(window, &Board::changeNodeRequested, this, &BoardController::showChangeNodeMenu);
    connect(diagram, &DiagramGraph::addBranchRequestApproved, m_command_manager, &CommandManager::addCreateBranchCommand);
    connect(m_command_manager, &CommandManager::createBranchCommandAdded, this, &BoardController::connectCreateBranchCommand);
    connect(diagram, &DiagramGraph::link_added, window, &Board::addLink);
    connect(diagram, &DiagramGraph::link_added, recovery_log, &recoveryLog::add_link_operation);
    //connect(window, &Board::connectionCreated, this, &BoardController::connectNewConnection);

    // Connect branch removal
    //connect(window, &Board::branchRemovalRequested, diagram, &DiagramGraph::processRemoveBranchRequest);
    //connect(diagram, &DiagramGraph::removeBranchRequestApproved, m_command_manager, &CommandManager::addRemoveBranchCommand);
    //connect(m_command_manager, &CommandManager::removeBranchCommandAdded, this, &BoardController::connectRemoveBranchCommand);
    connect(diagram, &DiagramGraph::link_removed, window, &Board::removeLink);
    connect(diagram, &DiagramGraph::link_removed, recovery_log, &recoveryLog::remove_link_operation);

    // Show messages
    connect(diagram, &DiagramGraph::error, mDialogFactory, &DialogFactory::showError);
    connect(window, &Board::error, mDialogFactory, &DialogFactory::showError);
    connect(this, &BoardController::message, mDialogFactory, &DialogFactory::showMessage);
    connect(this, &BoardController::error, mDialogFactory, &DialogFactory::showError);
    connect(this, &BoardController::warning, mDialogFactory, &DialogFactory::showWarning);

    // Connect board messages
    connect(window, &Board::message, mDialogFactory, &DialogFactory::showMessage);

    // Close window -> delete window
    connect(window, &QObject::destroyed, this, &QObject::deleteLater);

    // Connect removal
    connect(window, &Board::removalRequested, diagram, &DiagramGraph::procesRemoveRequest);
    connect(diagram, &DiagramGraph::removeRequestApproved, m_command_manager, &CommandManager::addRemoveCommand);
    connect(m_command_manager, &CommandManager::removeCommandAdded, this, &BoardController::connectRemoveCommand);

    // Connect node selection
    // connect(mConnectionHandler, &ConnectionHandler::selectionRequested, this, [this](ISelectable* selectable){ selectable->select(true);});
    // connect(mConnectionHandler, &ConnectionHandler::deselectionRequested, this, [this](ISelectable* selectable){ selectable->select(false);});

    setupActions();
}

BoardController::BoardController(const QString& title)
    : BoardController(new Board(title), new DiagramGraph()) {}

BoardController::~BoardController()
{
    qDebug() << "Deleted controller";
}

void BoardController::setupActions(){
    addMenuList("File", {
        {
        addAction("New project", this, [](){}, QKeySequence::New),
        addAction("Import project", this, [](){}, QKeySequence::Open)
        }, {
        addAction("Export as PNG", this, &BoardController::requestPNGPath),
        addAction("Export as JSON", this, &BoardController::requestJSONPath),
        addAction("Generate project", mDialogFactory->generateDialog, &QDialog::exec),
        }
    });
    addMenuList("Edit", {
        {
        addAction("Undo", m_command_manager, &CommandManager::undo, QKeySequence::Undo, QIcon::fromTheme(QIcon::ThemeIcon::EditUndo)),
        addAction("Redo", m_command_manager, &CommandManager::redo, QKeySequence::Redo, QIcon::fromTheme(QIcon::ThemeIcon::EditRedo)),
        }, {
        addCtrlAltAction("New class", window, &Board::onClassClicked, Qt::Key_C),
        addCtrlAltAction("New struct", window, &Board::onClassClicked, Qt::Key_S)
        }, {
        addAction("Delete", window, &Board::processRemovalRequest, Qt::Key_Backspace)
        }
    });
    addMenu("View", {
        addCheckableAction("Side bar", true, window->ui->side_menu, &QWidget::setVisible)
    });

    window->layout()->setMenuBar(menuBar);
}

void BoardController::requestJSONPath()
{
    QString path = mDialogFactory->selectJSON(getFileName());
    Serializer::instance().serialize(path, window, diagram);
}

void BoardController::requestPNGPath()
{
    QString path = mDialogFactory->selectPNG(getFileName());
    window->exportPNG(path);
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
    QString title = window->windowTitle().trimmed();
    title = title.toLower();
    title.replace(QRegularExpression("\\s+"), "_");
    title.remove(QRegularExpression("^_+|_+$"));
    return title;
}

void BoardController::connectCreateNodeCommand(std::shared_ptr<AddNodeCommand> command) {
    connect(command.get(), &AddNodeCommand::addNodeRequested, diagram, &DiagramGraph::addNode);
    connect(command.get(), &AddNodeCommand::removeNodeRequested, diagram, &DiagramGraph::removeNode);
}

// void BoardController::connectRemoveNodeCommand(std::shared_ptr<RemoveNodeCommand> command)
// {
//     connect(command.get(), &RemoveNodeCommand::addNodeRequested, diagram, &DiagramGraph::addNode);
//     connect(command.get(), &RemoveNodeCommand::removeNodeRequested, diagram, &DiagramGraph::removeNode);
//     connect(command.get(), &RemoveNodeCommand::addBranchRequested, diagram, &DiagramGraph::addBranch);
//     connect(command.get(), &RemoveNodeCommand::removeBranchRequested, diagram, &DiagramGraph::removeBranch);
// }

void BoardController::connectCreateBranchCommand(std::shared_ptr<AddBranchCommand> command) {
    connect(command.get(), &AddBranchCommand::addBranchRequested, diagram, &DiagramGraph::addBranch);
    connect(command.get(), &AddBranchCommand::removeBranchRequested, diagram, &DiagramGraph::removeBranch);
}

void BoardController::connectRemoveCommand(std::shared_ptr<RemoveCommand> command)
{
    qDebug() << "Added remove command";
    connect(command.get(), &RemoveCommand::addNodeRequested, diagram, &DiagramGraph::addNode);
    connect(command.get(), &RemoveCommand::removeNodeRequested, diagram, &DiagramGraph::removeNode);
    connect(command.get(), &RemoveCommand::addBranchRequested, diagram, &DiagramGraph::addBranch);
    connect(command.get(), &RemoveCommand::removeBranchRequested, diagram, &DiagramGraph::removeBranch);
}

// void BoardController::connectRemoveBranchCommand(std::shared_ptr<RemoveBranchCommand> command) {
//     connect(command.get(), &RemoveBranchCommand::addBranchRequested, diagram, &DiagramGraph::addBranch);
//     connect(command.get(), &RemoveBranchCommand::removeBranchRequested, diagram, &DiagramGraph::removeBranch);
// }

void BoardController::connectNewNodeView(CppClassView *item)
{
    //connect(item, &CppClassView::objectLeftClicked, mConnectionHandler, &ConnectionHandler::setSelected);
    //connect(item, &CppClassView::objectRightClicked, mConnectionHandler, &ConnectionHandler::handleNodeRightClick);
    connect(item, &CppClassView::objectMovedByUser, m_command_manager, &CommandManager::addMoveNodeCommand);
}

// void BoardController::connectNewConnection(Connection *connection)
// {
//     auto handleClick = [this, connection](){ mConnectionHandler->setSelected(connection); };
//     connect(connection->arrow, &Arrow::objectClicked, mConnectionHandler, handleClick);
//     connect(connection->line, &Line::objectClicked, mConnectionHandler, handleClick);
// }

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

void BoardController::showChangeNodeMenu(NodeViewPtr node)
{
    QList<QString> options = {"Add Field", "Add Method"};
    auto selected = mDialogFactory->showSelectionMenu<QString>(
        options,
        [](const QString& s) { return s; }
    );

    auto composition = node->get_uml_class_diagram_node();

    if(selected == std::optional<QString>{"Add Field"}) {
        auto field = mDialogFactory->createField();
        if(field) {
            composition->add_field(field);
            recovery_log->add_field_operation(composition.get(), field);
        }
    } else if(selected == std::optional<QString>{"Add Method"}) {
        auto method = mDialogFactory->createMethod();
        if(method) {
            composition->add_method(method);
            recovery_log->add_method_operation(composition.get(), method);
        }
    }
}

void BoardController::showBranchTypeMenu(NodeViewPtr from, NodeViewPtr to)
{
    auto fromNode = from->get_uml_class_diagram_node();
    auto toNode = to->get_uml_class_diagram_node();
    auto available = diagram->getAvailableBranchTypes(fromNode, toNode);

    auto selected = mDialogFactory->showSelectionMenu<BranchType>(
        available,
        [](const BranchType& t) {
            return QString::fromStdString(get_string_from_branch_type(t));
        }
    );

    if(selected) {
        diagram->processNewBranchRequest(
            fromNode.get(),
            toNode.get(),
            *selected
        );
        window->scene->clearSelection();
    }
}

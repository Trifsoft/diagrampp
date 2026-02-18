#include "Board.h"
#include "view/cpp_class_view.h"
#include "model/elements/composition/cpp_struct.h"
#include "model/elements/composition/cpp_class.h"
#include <QGraphicsView>
#include "src/ui/ui_Board.h"
#include "model/elements/type/regular_type.h"
#include <QFileDialog>
#include <QDir>
#include <QInputDialog>
#include "generate_project_dialog.h"
#include <QMessageBox>
#include <serializers/serializer.h>
#include <QRegularExpression>

#include <commandManager/command_manager.h>
#include "commandManager/add_node_command.h"
#include "commandManager/add_branch_command.h"
#include "commandManager/remove_branch_command.h"

Board::Board(const QString& project_name, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Board)
    , diagram(new DiagramGraph())
    , recovery_log(new recoveryLog())
    , mDialogFactory(new DialogFactory(this))
    , m_command_manager(new CommandManager())
    , mConnectionHandler(new ConnectionHandler())
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    ui->title->setText(project_name);

    scene = new QGraphicsScene();
    ui->board->setScene(scene);
    ui->board->setStyleSheet("background-color: #1a1a1a;");
    ui->top_bar->setStyleSheet("background-color: #252526;");
    ui->side_menu->setStyleSheet("background-color: #252526;");
    ui->back->setStyleSheet("color: #ffffff;");

    ui->linkageMode->setChecked(false);

    // Connect export json
    connect(ui->exportJSON, &QPushButton::clicked, this, &Board::requestJSONPath);
    connect(this, &Board::JSONPathRequested, mDialogFactory, &DialogFactory::selectJSON);
    connect(mDialogFactory, &DialogFactory::JSONPathSelected, this, &Board::requestSerialization);
    connect(this, &Board::serializationRequested, &Serializer::instance(), &Serializer::serialize);
    connect(&Serializer::instance(), &Serializer::serializationSuccess, this, &Board::showSerializationSuccessMessage);
    connect(&Serializer::instance(), &Serializer::serializationFailure, mDialogFactory, &DialogFactory::showError);

    // Connect export png
    connect(ui->exportPNG, &QPushButton::clicked, this, &Board::requestPNGPath);
    connect(this, &Board::PNGPathRequested, mDialogFactory, &DialogFactory::selectPNG);
    connect(mDialogFactory, &DialogFactory::PNGPathSelected, this, &Board::exportPNG);

    connect(ui->generate_project, &QPushButton::clicked, this, &Board::on_generate_project);

    // Connect radio buttons to command handler
    connect(ui->Inheritance, &QPushButton::clicked, this, &Board::onCheckRadioButtonClicked);
    connect(ui->Association, &QPushButton::clicked, this, &Board::onCheckRadioButtonClicked);
    connect(ui->Realization, &QPushButton::clicked, this, &Board::onCheckRadioButtonClicked);
    connect(ui->Aggregation, &QPushButton::clicked, this, &Board::onCheckRadioButtonClicked);
    connect(ui->Composition, &QPushButton::clicked, this, &Board::onCheckRadioButtonClicked);
    connect(ui->Dependency, &QPushButton::clicked, this, &Board::onCheckRadioButtonClicked);
    connect(this, &Board::radioButtonChecked, mConnectionHandler, &ConnectionHandler::updateSelectedBranchType);

    // Connect mode selection to command handler
    connect(ui->linkageMode, &QPushButton::clicked, this, &Board::onModeClicked);
    connect(ui->removeMode, &QPushButton::clicked, this, &Board::onModeClicked);
    connect(this, &Board::modeChanged, mConnectionHandler, &ConnectionHandler::updateSelectedNodeModification);

    // Connect node creation
    connect(ui->add_class, &QPushButton::clicked, mDialogFactory, &DialogFactory::handleClassClick);
    connect(ui->add_struct, &QPushButton::clicked, mDialogFactory, &DialogFactory::handleStructClick);
    connect(mDialogFactory, &DialogFactory::createNodeRequest, diagram, &DiagramGraph::processNewNodeRequest);
    connect(diagram, &DiagramGraph::addNodeRequestApproved, m_command_manager, &CommandManager::addCreateNodeCommand);
    connect(m_command_manager, &CommandManager::createNodeCommandAdded, this, &Board::connectCreateNodeCommand);
    connect(diagram, &DiagramGraph::node_added, this, &Board::addItem);

    // Connect node removal
    //connect(mConnectionHandler, &ConnectionHandler::nodeRemovalRequested, this, &Board::removeNode);
    connect(mConnectionHandler, &ConnectionHandler::nodeRemovalRequested, diagram, &DiagramGraph::processRemoveNodeRequest);
    connect(diagram, &DiagramGraph::removeNodeRequestApproved, m_command_manager, &CommandManager::addRemoveNodeCommand);
    connect(m_command_manager, &CommandManager::removeNodeCommandAdded, this, &Board::connectRemoveNodeCommand);
    connect(diagram, &DiagramGraph::node_removed, this, &Board::removeItem);
    connect(diagram, &DiagramGraph::node_removed, recovery_log, &recoveryLog::remove_node_operation);

    // Connect branch creation
    connect(mConnectionHandler, &ConnectionHandler::branchCreationRequested, diagram, &DiagramGraph::processNewBranchRequest);
    connect(diagram, &DiagramGraph::addBranchRequestApproved, m_command_manager, &CommandManager::addCreateBranchCommand);
    connect(m_command_manager, &CommandManager::createBranchCommandAdded, this, &Board::connectCreateBranchCommand);
    connect(diagram, &DiagramGraph::link_added, this, &Board::addLink);
    connect(diagram, &DiagramGraph::link_added, recovery_log, &recoveryLog::add_link_operation);

    // Connect branch removal
    connect(mConnectionHandler, &ConnectionHandler::branchRemovalRequested, diagram, &DiagramGraph::processRemoveBranchRequest);
    connect(diagram, &DiagramGraph::removeBranchRequestApproved, m_command_manager, &CommandManager::addRemoveBranchCommand);
    connect(m_command_manager, &CommandManager::removeBranchCommandAdded, this, &Board::connectRemoveBranchCommand);
    connect(diagram, &DiagramGraph::link_removed, this, &Board::removeLink);
    connect(diagram, &DiagramGraph::link_removed, recovery_log, &recoveryLog::remove_link_operation);

    // Show messages
    connect(diagram, &DiagramGraph::error, mDialogFactory, &DialogFactory::showError);
    connect(this, &Board::message, mDialogFactory, &DialogFactory::showMessage);
    connect(this, &Board::error, mDialogFactory, &DialogFactory::showError);

    setup_actions();
}

void Board::setup_actions(){
    auto undoAction = new QAction("Undo", this);
    undoAction->setShortcut(QKeySequence::Undo);

    connect(undoAction, &QAction::triggered,
            this, [this]() {
                m_command_manager->undo();
            });

    addAction(undoAction);

    auto redoAction = new QAction("Redo", this);
    redoAction->setShortcut(QKeySequence::Redo);

    connect(redoAction, &QAction::triggered,
            this, [this]() {
                m_command_manager->redo();
            });

    addAction(redoAction);

}

DiagramGraph* Board::get_diagram() const {
    return diagram;
}

CppClassView* Board::get_view_from_node(SharedNodePtr node) const {
    for(auto& addedNode : nodes) {
        if(addedNode->get_uml_class_diagram_node() == node) {
            return addedNode;
        }
    }
    for(auto& removedNode : removedNodes) {
        if(removedNode->get_uml_class_diagram_node() == node) {
            return removedNode;
        }
    }
    return nullptr;
}

std::shared_ptr<Connection> Board::get_connection_from_nodes(CppClassView* from,
                                                             CppClassView* to,
                                                             BranchType branchType) const
{
    const auto& connectionList = nodeMap[from];
    for(auto it = connectionList.begin(); it != connectionList.end(); it++) {
        if(it->first == to && it->second->branchType() == branchType) {
            return it->second;
        }
    }
    for(auto& [iFrom, iTo, iConn] : removedBranches) {
        if(iFrom == from && iTo == to && iConn->branchType() == branchType) {
            return iConn;
        }
    }
    return nullptr;
}

QList<std::shared_ptr<Connection>> Board::getIncomingConnections(CppClassView* node) const
{
    QList<std::shared_ptr<Connection>> result;
    for(auto it = nodeMap.begin(); it != nodeMap.end(); it++) {
        if(it.key() == node) {
            continue;
        }
        for(auto& [iTo, iConn] : it.value()) {
            if(iTo == node) {
                result.append(iConn);
            }
        }
    }
    return result;
}

Board::~Board()
{
    delete diagram;
    delete recovery_log;
    delete mDialogFactory;
    delete m_command_manager;
    delete mConnectionHandler;
    delete scene;
    delete ui;
}

void Board::onCheckRadioButtonClicked(){
    BranchType branchType;
    if(ui->Inheritance->isChecked()){
        branchType = BranchType::INHERITANCE;
    }else if(ui->Association->isChecked()){
        branchType = BranchType::ASSOCIATION;
    }else if(ui->Realization->isChecked()){
        branchType = BranchType::REALIZATION;
    }else if(ui->Aggregation->isChecked()){
        branchType = BranchType::AGGREGATION;
    }else if(ui->Composition->isChecked()){
        branchType = BranchType::COMPOSITION;
    }else{
        branchType = BranchType::DEPENDENCY;
    }
    emit radioButtonChecked(branchType);
}

void Board::onModeClicked(){
    auto oldMode = mConnectionHandler->getSelectedNodeModification();
    NodeModification newMode;
    if(oldMode == NodeModification::None) {
        newMode = ui->linkageMode->isChecked() ? NodeModification::Link : NodeModification::Remove;
    }
    else if(oldMode == NodeModification::Link) {
        newMode = ui->removeMode->isChecked() ? NodeModification::Remove : NodeModification::None;
    }
    else {
        newMode = ui->linkageMode->isChecked() ? NodeModification::Link : NodeModification::None;
    }
    ui->linkageMode->setChecked(newMode == NodeModification::Link);
    ui->removeMode->setChecked(newMode == NodeModification::Remove);
    emit modeChanged(newMode);
}


void Board::on_add_field_requested(Composition *node, std::shared_ptr<Field> field)
{
    if(node)
    {
        node->add_field(field);
    }

    qDebug() << "Recieved signal add field from: " << node->get_label();
}

void Board::on_add_method_requested(Composition *node, std::shared_ptr<Method> method)
{
    if(node)
    {
        node->add_method(method);
    }

    qDebug() << "Recieved signal add method from: " << node->get_label();
}

void Board::on_edit_field_requested(Composition *node, std::weak_ptr<Field> old_field_weak,  std::shared_ptr<Field> new_field)
{
    if(node){
        // old_field is now shared_ptr
        if(auto old_field = old_field_weak.lock()){
            *old_field = *new_field;
        }
    }
    qDebug() << "Received signal edit field from: " << node->get_label();
}

void Board::on_edit_method_requested(Composition *node, std::weak_ptr<Method> old_method_weak, std::shared_ptr<Method>new_method)
{
    if(node){
        // old_field is now shared_ptr
        if(auto old_method = old_method_weak.lock()){
            *old_method = *new_method;
        }
    }

    qDebug() << "Received signal edit method from: " << node->get_label();
}

void Board::add_item(SharedNodePtr node, double x, double y) {
    auto item = get_view_from_node(node);
    if(!item) {
        item = new CppClassView(node);

        connect(item, &CppClassView::objectClicked, mConnectionHandler, &ConnectionHandler::handleNodeClick);

        connect(item, &CppClassView::add_field_request, recovery_log, &recoveryLog::add_field_operation);
        connect(item, &CppClassView::add_method_request, recovery_log, &recoveryLog::add_method_operation);

        connect(item, &CppClassView::add_field_request, this, &Board::on_add_field_requested);
        connect(item, &CppClassView::add_method_request, this, &Board::on_add_method_requested);
        connect(item, &CppClassView::edit_field_request, this, &Board::on_edit_field_requested);
        connect(item, &CppClassView::edit_method_request, this, &Board::on_edit_method_requested);

        item->setX(x);
        item->setY(y);
    }

    mAddNode(item);
}

void Board::addItem(SharedNodePtr node) {
    add_item(node, 0, 0);
}

void Board::exportPNG(const QString& filePath){

    QRectF items_rect = scene->itemsBoundingRect();
    if(items_rect.isEmpty()){
        QMessageBox::warning(this, "Export", "No items to export!");
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

void Board::requestJSONPath()
{
    emit JSONPathRequested(get_file_name());
}

void Board::requestPNGPath()
{
    emit PNGPathRequested(get_file_name());
}

void Board::requestSerialization(const QString &path) { emit serializationRequested(path, this); }

void Board::showSerializationSuccessMessage(const QString& filePath)
{
    emit message("Success", "Diagram exported to:\n" + filePath);
}

void Board::on_generate_project(){
    generateProjectDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted){
        std::string path = dialog.get_selected_path().toStdString();
        ProjectGenerator::FileNameNotation notation = dialog.get_selected_notation();
        ProjectGenerator::ReplaceToggle toggle = dialog.get_selected_toggle();
        std::string project_dir_name = dialog.get_project_dir_name().toStdString();
        ProjectGenerator::GenerationStatusCode status = ProjectGenerator::generate(
            diagram->get_diagram(),
            path,
            project_dir_name,
            notation,
            toggle
            );

        if(status != ProjectGenerator::GenerationStatusCode::OK){
            QString err_message;
            switch (status)
            {
            case ProjectGenerator::GenerationStatusCode::EXISTING_PROJECT_DIR_ON_PATH:
                err_message = "Directory with a given name already exists on path. Choose a different name or replace it.";
                break;
            case ProjectGenerator::GenerationStatusCode::PERMISSION_DENIED:
                err_message = "Permission denied.";
                break;
            case ProjectGenerator::GenerationStatusCode::NO_MEMORY_SPACE:
                err_message = "No enough memory on the disc.";
                break;
            case ProjectGenerator::GenerationStatusCode::FILE_NOT_CREATED:
                err_message = "Error while generating file.";
                break;
            case ProjectGenerator::GenerationStatusCode::NO_SUCH_DIR:
                err_message = "The selected path does not exist. Please choose an existing directory.";
                break;
            default:
                err_message = "";
            }
            QMessageBox::warning(this, "Warning", err_message);
        }else{
            QMessageBox::information(this, "Success", "Project is generated on path: \n" + QString::fromStdString(path));
        }
    }
}

void Board::set_title(const QString& title) {
    ui->title->setText(title);
}

QString& Board::get_file_name() {
    static QString file_name;
    QString title = ui->title->text().trimmed();
    title = title.toLower();
    title.replace(QRegularExpression("\\s+"), "_");
    title.remove(QRegularExpression("^_+|_+$"));
    file_name = title;
    return file_name;
}

void Board::addLink(SharedNodePtr from, SharedNodePtr to, BranchType branch) {
    auto child = get_view_from_node(from);
    auto parent = get_view_from_node(to);
    auto connection = get_connection_from_nodes(child, parent, branch);
    if(child && parent && !connection){
        Arrow* arrow = get_arrow(parent, branch);

        Line* line = new Line(branch, QLineF(child->get_top_center(), arrow->get_bottom_center()));
        connection = std::make_shared<Connection>(arrow, line);

        connect(arrow, &Arrow::moved_by, line, &Line::move_end);
        connect(child, &CppClassView::moved_by, line, &Line::move_start);

        connect(child, &QObject::destroyed, connection.get(), &QObject::deleteLater);
        connect(parent, &QObject::destroyed, connection.get(), &QObject::deleteLater);
    }
    mAddBranch(child, parent, connection);
}

void Board::removeLink(SharedNodePtr from, SharedNodePtr to, BranchType branch) {
    auto child = get_view_from_node(from);
    auto parent = get_view_from_node(to);
    auto connection = get_connection_from_nodes(child, parent, branch);
    mRemoveBranch(child, parent, connection);
}

void Board::removeItem(SharedNodePtr target) {
    auto node = get_view_from_node(target);
    mRemoveNode(node);
}

Arrow* Board::get_arrow(CppClassView* view, BranchType branch) {
    const auto& incomingConnections = getIncomingConnections(view);
    Arrow* new_arrow = new Arrow(branch, view->pos(), view->boundingRect().height(), incomingConnections.size());
    for(auto& connection : incomingConnections) {
        connect(connection.get(), &QObject::destroyed, new_arrow, &Arrow::move_back);
    }
    connect(view, &CppClassView::moved_by, new_arrow, &Arrow::move_by);
    connect(view, &CppClassView::height_changed_by, new_arrow, &Arrow::move_y);
    return new_arrow;
}

void Board::connectCreateNodeCommand(std::shared_ptr<AddNodeCommand> command) {
    connect(command.get(), &AddNodeCommand::addNodeRequested, diagram, &DiagramGraph::addNode);
    connect(command.get(), &AddNodeCommand::removeNodeRequested, diagram, &DiagramGraph::removeNode);
}

void Board::connectRemoveNodeCommand(std::shared_ptr<RemoveNodeCommand> command)
{
    connect(command.get(), &RemoveNodeCommand::addNodeRequested, diagram, &DiagramGraph::addNode);
    connect(command.get(), &RemoveNodeCommand::removeNodeRequested, diagram, &DiagramGraph::removeNode);
    connect(command.get(), &RemoveNodeCommand::addBranchRequested, diagram, &DiagramGraph::addBranch);
    connect(command.get(), &RemoveNodeCommand::removeBranchRequested, diagram, &DiagramGraph::removeBranch);
}

void Board::connectCreateBranchCommand(std::shared_ptr<AddBranchCommand> command) {
    connect(command.get(), &AddBranchCommand::addBranchRequested, diagram, &DiagramGraph::addBranch);
    connect(command.get(), &AddBranchCommand::removeBranchRequested, diagram, &DiagramGraph::removeBranch);
}

void Board::connectRemoveBranchCommand(std::shared_ptr<RemoveBranchCommand> command) {
    connect(command.get(), &RemoveBranchCommand::addBranchRequested, diagram, &DiagramGraph::addBranch);
    connect(command.get(), &RemoveBranchCommand::removeBranchRequested, diagram, &DiagramGraph::removeBranch);
}

void Board::onAddBranch(CppClassView*,
                        CppClassView*,
                        std::shared_ptr<Connection> connection)
{
    scene->addItem(connection->arrow);
    scene->addItem(connection->line);
}

void Board::onRemoveBranch(CppClassView*,
                           CppClassView*,
                           std::shared_ptr<Connection> connection)
{
    scene->removeItem(connection->arrow);
    scene->removeItem(connection->line);
}

void Board::onAddNode(CppClassView* node)
{
    scene->addItem(node);
}

void Board::onRemoveNode(CppClassView* node)
{
    scene->removeItem(node);
}

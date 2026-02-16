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
#include <fstream>
#include "serializers/diagram_json_serializer.h"

#include <commandManager/command_manager.h>
#include "commandManager/add_node_command.h"
#include "commandManager/remove_node_with_branches_command.h"
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

    scene = new QGraphicsScene(this);
    ui->board->setScene(scene);
    ui->board->setStyleSheet("background-color: #1a1a1a;");
    ui->top_bar->setStyleSheet("background-color: #252526;");
    ui->side_menu->setStyleSheet("background-color: #252526;");
    ui->back->setStyleSheet("color: #ffffff;");


    ui->linkageMode->setChecked(false);

    // signals for connecting graph with Board
    connect(diagram, &DiagramGraph::link_removed, this, &Board::on_link_removed);
    //connect(diagram, &DiagramGraph::node_added, this, &Board::on_node_added);
    connect(diagram, &DiagramGraph::node_removed, this, &Board::on_node_removed);


    // signals for log file
    connect(diagram, &DiagramGraph::link_added, recovery_log, &recoveryLog::add_link_operation);
    connect(diagram, &DiagramGraph::link_removed, recovery_log, &recoveryLog::remove_link_operation);
    connect(diagram, &DiagramGraph::node_removed, recovery_log, &recoveryLog::remove_node_operation);

    connect(ui->exportJSON, &QPushButton::clicked, this, &Board::exportJSON);
    connect(ui->exportPNG, &QPushButton::clicked, this, &Board::exportPNG);
    connect(ui->generate_project, &QPushButton::clicked, this, &Board::on_generate_project);

    // Connect node creation
    connect(ui->add_class, &QPushButton::clicked, mDialogFactory, &DialogFactory::handleClassClick);
    connect(ui->add_struct, &QPushButton::clicked, mDialogFactory, &DialogFactory::handleStructClick);
    connect(mDialogFactory, &DialogFactory::createNodeRequest, diagram, &DiagramGraph::processNewNodeRequest);
    connect(diagram, &DiagramGraph::addNodeRequestApproved, m_command_manager, &CommandManager::addCreateNodeCommand);
    connect(m_command_manager, &CommandManager::createNodeCommandAdded, this, &Board::connectCreateNodeCommand);
    connect(diagram, &DiagramGraph::node_added, this, &Board::addItem);

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

    // Connect node removal
    connect(mConnectionHandler, &ConnectionHandler::nodeRemovalRequested, this, &Board::removeNode);

    // Connect branch creation
    connect(mConnectionHandler, &ConnectionHandler::branchCreationRequested, diagram, &DiagramGraph::processNewBranchRequest);
    connect(diagram, &DiagramGraph::addBranchRequestApproved, m_command_manager, &CommandManager::addCreateBranchCommand);
    connect(m_command_manager, &CommandManager::createBranchCommandAdded, this, &Board::connectCreateBranchCommand);
    connect(diagram, &DiagramGraph::link_added, this, &Board::addLink);

    // Connect branch removal
    connect(mConnectionHandler, &ConnectionHandler::branchRemovalRequested, this, &Board::removeBranch);

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

CppClassView* Board::get_view_from_node(SharedNodePtr node) {
    for(auto view : views) {
        if(view->get_uml_class_diagram_node() == node.get()) {
            return view;
        }
    }
    return nullptr;
}

Board::~Board()
{
    delete diagram;
    delete recovery_log;
    qDeleteAll(views);
    views.clear();
    delete mDialogFactory;
    delete m_command_manager;
    delete mConnectionHandler;
    delete ui;
    delete scene;
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

void Board::execute_remove_node_with_branches(SharedNodePtr node)
{
    auto cmd = std::make_shared<RemoveNodeWithBranchesCommand>(diagram, node);
    m_command_manager->execute(cmd);
}

void Board::execute_remove_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type)
{
    auto cmd = std::make_shared<RemoveBranchCommand>(diagram, from, to, branch_type);
    m_command_manager->execute(cmd);
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
    CppClassView* item = new CppClassView(node);

    item->setX(x);
    item->setY(y);

    Q_ASSERT(node);
    Q_ASSERT(item);

    views.append(item);
    scene->addItem(item);
    //execute_add_node(node);

    connect(item, &CppClassView::objectClicked, mConnectionHandler, &ConnectionHandler::handleNodeClick);

    connect(item, &CppClassView::add_field_request, recovery_log, &recoveryLog::add_field_operation);
    connect(item, &CppClassView::add_method_request, recovery_log, &recoveryLog::add_method_operation);

    connect(item, &CppClassView::add_field_request, this, &Board::on_add_field_requested);
    connect(item, &CppClassView::add_method_request, this, &Board::on_add_method_requested);
    connect(item, &CppClassView::edit_field_request, this, &Board::on_edit_field_requested);
    connect(item, &CppClassView::edit_method_request, this, &Board::on_edit_method_requested);
}

void Board::addItem(SharedNodePtr node) {
    add_item(node, 0, 0);
}

void Board::exportPNG(){
    QString file_name = QFileDialog::getSaveFileName(
            this,
            "Export Diagram as PNG",
            QDir::homePath() + "/diagram_export.png",
            "PNG Images (*.png);;All Files (*)"
        );

    if(file_name.isEmpty()){
        return;
    }

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

    if(image.save(file_name, "PNG", 100)){
        QMessageBox::information(this, "Success",
                                 QString("Diagram exported to:\n%1\n\nSize: %2x%3 pixels")
                                        .arg(file_name).arg(image.width()).arg(image.height()));
    }else{
        QMessageBox::critical(this, "Error", "Failed to save image. Check write permissions.");
    }
}

void Board::exportJSON() {
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Export Diagram as JSON",
        QDir::homePath() + "/" + get_file_name() + ".json",
        "JSON Files (*.json)"
    );

    if (filePath.isEmpty()) {
        return;
    }

    if (!filePath.endsWith(".json", Qt::CaseInsensitive)) {
        filePath += ".json";
    }

    std::ofstream outputStream(filePath.toStdString());
    if (!outputStream.is_open()) {
        QMessageBox::critical(this, "Error", "Failed to open file for writing: " + filePath);
        return;
    }

    DiagramJsonSerializer serializer(this);
    serializer.serialize(outputStream);

    outputStream.close();

    QMessageBox::information(this, "Success", "Diagram exported to:\n" + filePath);
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
    if(child && parent){
        Arrow* arrow = get_arrow(parent, branch);

        Line* line = new Line(branch, QLineF(child->get_top_center(), arrow->get_bottom_center()));
        Connection* connection = new Connection(arrow, line);

        connect(arrow, &Arrow::moved_by, line, &Line::move_end);
        connect(child, &CppClassView::moved_by, line, &Line::move_start);

        connect(child, &QObject::destroyed, connection, &QObject::deleteLater);
        connect(parent, &QObject::destroyed, connection, &QObject::deleteLater);

        connections[{child, parent, branch}] = connection;
        outgoingConnectionInfo[child].append({child, parent, branch});
        incomingConnectionInfo[parent].append({child, parent, branch});

        scene->addItem(arrow);
        scene->addItem(line);
    }
}

void Board::on_link_removed(SharedNodePtr from, SharedNodePtr to, BranchType branch) {
    auto child = get_view_from_node(from);
    auto parent = get_view_from_node(to);
    if(child && parent && connections.contains({child, parent, branch})){
        auto conn = connections.take({child, parent, branch});
        incomingConnectionInfo[parent].removeAll({child, parent, branch});
        outgoingConnectionInfo[child].removeAll({child, parent, branch});
        delete conn;
    }
}

void Board::on_node_removed(SharedNodePtr target) {
    auto node = get_view_from_node(target);
    views.removeAll(node);
    if(node){
        delete node;
        if(incomingConnectionInfo.contains(node)) {
            auto info = incomingConnectionInfo.take(node);
            for(auto it = info.begin(); it != info.end(); it++) {
                connections.remove(*it);
            }
        }
        if(outgoingConnectionInfo.contains(node)) {
            auto info = incomingConnectionInfo.take(node);
            for(auto it = info.begin(); it != info.end(); it++) {
                connections.remove(*it);
            }
        }
    }
}

Arrow* Board::get_arrow(CppClassView* view, BranchType branch) {
    Arrow* new_arrow = new Arrow(branch, view->pos(), view->boundingRect().height(), incomingConnectionInfo[view].size());
    for(auto info : incomingConnectionInfo[view]) {
        auto connection = connections[info];
        connect(connection, &QObject::destroyed, new_arrow, &Arrow::move_back);
    }
    connect(view, &CppClassView::moved_by, new_arrow, &Arrow::move_by);
    connect(view, &CppClassView::height_changed_by, new_arrow, &Arrow::move_y);
    return new_arrow;
}

void Board::connectCreateNodeCommand(std::shared_ptr<AddNodeCommand> command) {
    connect(command.get(), &AddNodeCommand::addNodeRequested, diagram, &DiagramGraph::addNode);
    connect(command.get(), &AddNodeCommand::removeNodeRequested, diagram, &DiagramGraph::removeNode);
}

void Board::connectCreateBranchCommand(std::shared_ptr<AddBranchCommand> command) {
    connect(command.get(), &AddBranchCommand::addBranchRequested, diagram, &DiagramGraph::addBranch);
    connect(command.get(), &AddBranchCommand::removeBranchRequested, diagram, &DiagramGraph::removeBranch);
}

void Board::removeNode(NodePtr node)
{
    auto sharedNode = diagram->find_pointer_owner(node);
    execute_remove_node_with_branches(sharedNode);
}

void Board::removeBranch(NodePtr from, NodePtr to, BranchType branchType)
{
    auto fromShared = diagram->find_pointer_owner(from);
    auto toShared = diagram->find_pointer_owner(to);
    execute_remove_branch(fromShared, toShared, branchType);
}

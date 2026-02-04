#include "Board.h"
#include "view/cpp_class.h"
#include "model/elements/composition/cpp_struct.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_enum.h"
#include <QGraphicsView>
#include "src/ui/ui_Board.h"
#include "model/elements/type/regular_type.h"
#include "nodefactory.h"
#include <QFileDialog>
#include <QDir>
#include "generate_project_dialog.h"
#include <QMessageBox>
#include <validator.h>

Board::Board(QWidget *parent)
    : QWidget(parent), ui(new Ui::Board), diagram(new DiagramGraph()), signal_processor(new signalProcessor(this)),
    recovery_log(new recoveryLog())
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    scene = new QGraphicsScene(this);
    ui->board->setScene(scene);
    ui->board->setStyleSheet("background-color: white");
    ui->side_menu->setStyleSheet("background-color: #2c3e50;");
    ui->linkageMode->setChecked(false);

    // signals for GUI
    connect(diagram, &DiagramGraph::link_added, signal_processor, &signalProcessor::add_link_process);
    connect(diagram, &DiagramGraph::link_removed, signal_processor, &signalProcessor::remove_link_process);
    connect(diagram, &DiagramGraph::node_removed, signal_processor, &signalProcessor::remove_node_process);
    // signals for log file
    connect(diagram, &DiagramGraph::link_added, recovery_log, &recoveryLog::add_link_operation);
    connect(diagram, &DiagramGraph::link_removed, recovery_log, &recoveryLog::remove_link_operation);
    connect(diagram, &DiagramGraph::node_removed, recovery_log, &recoveryLog::remove_node_operation);

    connect(ui->exportPNG, &QPushButton::clicked, this, &Board::exportPNG);
    connect(ui->generate_project, &QPushButton::clicked, this, &Board::on_generate_project);

    // Connect buttons to slots
    connect(ui->add_class, &QPushButton::clicked, this, &Board::onAddClassClicked);
    connect(ui->add_interface, &QPushButton::clicked, this, &Board::onAddInterfaceClicked);
    connect(ui->add_enum, &QPushButton::clicked, this, &Board::onAddEnumClicked);
    connect(ui->Inheritance, &QPushButton::clicked, this, &Board::onCheckRadioButtonClicked);
    connect(ui->Association, &QPushButton::clicked, this, &Board::onCheckRadioButtonClicked);
    connect(ui->Realization, &QPushButton::clicked, this, &Board::onCheckRadioButtonClicked);
    connect(ui->Aggregation, &QPushButton::clicked, this, &Board::onCheckRadioButtonClicked);
    connect(ui->Composition, &QPushButton::clicked, this, &Board::onCheckRadioButtonClicked);
    connect(ui->Dependency, &QPushButton::clicked, this, &Board::onCheckRadioButtonClicked);
    connect(ui->linkageMode, &QPushButton::clicked, this, &Board::onModeClicked);
    connect(ui->removeMode, &QPushButton::clicked, this, &Board::onModeClicked);
}

DiagramGraph* Board::get_diagram() const {
    return diagram;
}

CppClass* Board::get_view_from_node(SharedNodePtr node) {
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
    delete signal_processor;
    delete recovery_log;
    qDeleteAll(views);
    views.clear();
    delete ui;
    delete scene;
}

void Board::onCheckRadioButtonClicked(){
    if(ui->Inheritance->isChecked()){
        branch_type = BranchType::INHERITANCE;
    }else if(ui->Association->isChecked()){
        branch_type = BranchType::ASSOCIATION;
    }else if(ui->Realization->isChecked()){
        branch_type = BranchType::REALIZATION;
    }else if(ui->Aggregation->isChecked()){
        branch_type = BranchType::AGGREGATION;
    }else if(ui->Composition->isChecked()){
        branch_type = BranchType::COMPOSITION;
    }else{
        branch_type = BranchType::DEPENDENCY;
    }
}

void Board::onModeClicked(){
    linkageMode = ui->linkageMode->isChecked();
    removeMode = ui->removeMode->isChecked();
    if(linkageMode && removeMode){
        ui->linkageMode->setChecked(false);
        ui->removeMode->setChecked(false);
        linkageMode = removeMode = false;
    }
    first_activated = second_activated = nullptr;
}


void Board::add_node_relationship(SharedNodePtr node, SharedNodePtr neighbour, BranchType branch_type){
    const bool connection_exists = diagram->connection_exists(node, neighbour, branch_type);
    if(!connection_exists){
        std::string error_message;
        diagram->add_branch(node, neighbour, branch_type);
        if(!Validator::validate(error_message, node, neighbour, branch_type, diagram->get_diagram())){
            QMessageBox::warning(this, "Warning", QString::fromStdString(error_message));
            diagram->remove_branch(node, neighbour, branch_type);
        }
    }else {
        QMessageBox::warning(this, "Warning", "Connection already exists.");
    }
}


void Board::on_object_clicked(Composition* clicked_object){
    if(!linkageMode && !removeMode){
        return;
    }
    if(!first_activated){
        first_activated = diagram->find_pointer_owner(clicked_object);
        return;
    }
    second_activated = diagram->find_pointer_owner(clicked_object);

    if((first_activated == second_activated) && removeMode){
        diagram->remove_node(first_activated);
        first_activated = second_activated = nullptr;
        return;
    }

    if((first_activated && second_activated) && linkageMode){
        add_node_relationship(first_activated, second_activated, branch_type);
    }else if((first_activated && second_activated) && removeMode){
        if(!diagram->connection_exists(first_activated, second_activated, branch_type)){
            QMessageBox::warning(this, "Warning", QString::fromStdString("Connection doesn't exsist"));
        }else{
            diagram->remove_branch(first_activated, second_activated, branch_type);
        }
    }

    #if DEBUG_MODE>=1
        qDebug() << "";
        diagram->show_diagram();
    #endif

        first_activated = second_activated = nullptr;
}

void Board::onAddClassClicked()     { openNodeFactory(NodeType::Class);  }
void Board::onAddInterfaceClicked() { openNodeFactory(NodeType::Struct); }
void Board::onAddEnumClicked()      { openNodeFactory(NodeType::Enum);   }

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

// void Board::add_item(std::shared_ptr<Composition> node) {   //TODO [Nikola] - izmeniti da bude IUMLClassDiagramNode umesto Composition
//     CppClass* item = new CppClass(this, node);
//     scene->addItem(item);
//     diagram->add_node(item);

//     connect(item, &CppClass::objectClicked, this, &Board::on_object_clicked);
// }


// [REFACTOR] implement get_element_by_id(int id) in Composition
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

// [REFACTOR] implement get_element_by_id(int id) in Composition
void Board::on_edit_method_requested(Composition *node, std::weak_ptr<Method> old_method_weak, std::shared_ptr<Method>new_method)
{
    // if(node)
    // {
    //     for (int i = 0; i < node->methods.count(); ++i) {
    //         if (node->methods[i]->get_id() == method_id) {
    //             node->methods[i] = new_method;
    //             break;
    //         }
    //     }
    // }

    if(node){
        // old_field is now shared_ptr
        if(auto old_method = old_method_weak.lock()){
            *old_method = *new_method;
        }
    }

    qDebug() << "Received signal edit method from: " << node->get_label();
}


void Board::openNodeFactory(NodeType node_type) {

    // [FIX] Probably a leak
    NodeFactory* node_factory = new NodeFactory(node_type);
    connect(node_factory, &NodeFactory::generateClicked, this, &Board::onGenerateClicked);
    node_factory->show();
}

void Board::onGenerateClicked(const QString& class_name, NodeType node_type) {
    switch(node_type) {
        case NodeType::Class: {
            auto new_node = std::make_shared<CPPClass>(class_name);
            // new_node->add_field("test1", std::make_shared<RegularType>(RegularType("int")));
            // new_node->add_field("test2", std::make_shared<RegularType>(RegularType("bool")));
            // new_node->add_field("test3", std::make_shared<RegularType>(RegularType("float")));
            // new_node->add_field("test4", std::make_shared<RegularType>(RegularType("double")));
            // new_node->add_method("foo", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });


            new_node->add_field("test1", "tip1", Visibility::Protected);
            new_node->add_field("test2", "tip2", Visibility::Private);
            new_node->add_field("test3", "tip3");
            new_node->add_method("method","void", MethodKind::Regular, { new Argument("bar", "int") }, Visibility::Private);


            add_item(new_node);
            break;
        }
        case NodeType::Struct: {
            auto new_node = std::make_shared<CPPStruct>(class_name);
            // new_node->add_field("test1", std::make_shared<RegularType>(RegularType("int")));
            // new_node->add_field("test2", std::make_shared<RegularType>(RegularType("bool")));
            // new_node->add_field("test3", std::make_shared<RegularType>(RegularType("float")));
            // new_node->add_field("test4", std::make_shared<RegularType>(RegularType("double")));
            // new_node->add_method("foo", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });


            new_node->add_field("test1", "tip1", Visibility::Protected);
            new_node->add_field("test2", "tip2", Visibility::Private);
            new_node->add_field("test3", "tip3");
            new_node->add_method("method","void", MethodKind::Regular, { new Argument("bar", "int") }, Visibility::Private);

            add_item(new_node);
            break;
        }
        case NodeType::Enum: {
            auto new_node = std::make_shared<CPPClass>(class_name);
            // new_node->add_field("test1", std::make_shared<RegularType>(RegularType("int")));
            // new_node->add_field("test2", std::make_shared<RegularType>(RegularType("bool")));
            // new_node->add_field("test3", std::make_shared<RegularType>(RegularType("float")));
            // new_node->add_field("test4", std::make_shared<RegularType>(RegularType("double")));
            // new_node->add_method("foo", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });

            new_node->add_field("test1", "tip1", Visibility::Protected);
            new_node->add_field("test2", "tip2", Visibility::Private);
            new_node->add_field("test3", "tip3");
            new_node->add_method("method","void", MethodKind::Regular, { new Argument("bar", "int") }, Visibility::Private);


            add_item(new_node);
            break;
        }
    }

}
void Board::add_item(std::shared_ptr<Composition> node, const double coord_x, const double coord_y) {   //TODO [Nikola] - izmeniti da bude IUMLClassDiagramNode umesto Composition
    CppClass* item = new CppClass(this, node);
    if(coord_x && coord_y){
        item->setX(coord_x);
        item->setY(coord_y);
    }

    scene->addItem(item);
    //dynamic_cast<CPPStruct*>(item->getClassDiagramNode().get())->parent = item;
    diagram->add_node(node);
    views.append(item);

    connect(item, &CppClass::objectClicked, this, &Board::on_object_clicked);


    connect(item, &CppClass::add_field_request, this, &Board::on_add_field_requested);
    connect(item, &CppClass::add_method_request, this, &Board::on_add_method_requested);
    connect(item, &CppClass::edit_field_request, this, &Board::on_edit_field_requested);
    connect(item, &CppClass::edit_method_request, this, &Board::on_edit_method_requested);
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

void Board::on_generate_project(){
    generateProjectDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted){
        std::string path = dialog.get_selected_path().toStdString();
        ProjectGenerator::FileNameNotation notation = dialog.get_selected_notation();
        ProjectGenerator::ReplaceToggle toggle = dialog.get_selected_toggle();
        std::string project_dir_name = ui->title->text().toStdString();
        if(!path.empty()){
            ProjectGenerator::GenerationStatusCode status = ProjectGenerator::generate(
                diagram->get_diagram(),
                path,
                project_dir_name,
                notation,
                toggle
                );
        }
    }
}

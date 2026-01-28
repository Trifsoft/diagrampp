#include "Board.h"
#include "view/cpp_class.h"
#include "model/elements/composition/cpp_struct.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_enum.h"
#include <QGraphicsView>
#include "src/ui/ui_Board.h"
#include "model/elements/type/regular_type.h"
#include "nodefactory.h"
#include "image_paths.h"
#include <QMessageBox>

Board::Board(QWidget *parent)
    : QWidget(parent), ui(new Ui::Board), diagram(new DiagramGraph()), signal_processor(new signalProcessor())
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

    // Connect buttons to slots
    connect(ui->add_class, &QPushButton::clicked, this, &Board::onAddClassClicked);
    connect(ui->add_interface, &QPushButton::clicked, this, &Board::onAddInterfaceClicked);
    connect(ui->add_enum, &QPushButton::clicked, this, &Board::onAddEnumClicked);
    connect(ui->Inheritance, &QPushButton::clicked, this, &Board::onCheckRadioButtonToggled);
    connect(ui->Association, &QPushButton::clicked, this, &Board::onCheckRadioButtonToggled);
    connect(ui->Navigation, &QPushButton::clicked, this, &Board::onCheckRadioButtonToggled);
    connect(ui->Aggregation, &QPushButton::clicked, this, &Board::onCheckRadioButtonToggled);
    connect(ui->Composition, &QPushButton::clicked, this, &Board::onCheckRadioButtonToggled);
    connect(ui->Dependency, &QPushButton::clicked, this, &Board::onCheckRadioButtonToggled);
    connect(ui->linkageMode, &QPushButton::clicked, this, &Board::onModeClicked);
    connect(ui->removeMode, &QPushButton::clicked, this, &Board::onModeClicked);
}

Board::~Board()
{
    delete diagram;
    delete ui;
    delete scene;
}

void Board::onCheckRadioButtonToggled(){
    if(ui->Inheritance->isChecked()){
        branchType = BranchType::INHERITANCE;
    }else if(ui->Association->isChecked()){
        branchType = BranchType::ASSOCIATION;
    }else if(ui->Navigation->isChecked()){
        branchType = BranchType::NAVIGATION;
    }else if(ui->Aggregation->isChecked()){
        branchType = BranchType::AGGREGATION;
    }else if(ui->Composition->isChecked()){
        branchType = BranchType::COMPOSITION;
    }else{
        branchType = BranchType::DEPENDENCY;
    }
}

void Board::onModeClicked(){
    linkageMode = ui->linkageMode->isChecked();
    removeMode = ui->removeMode->isChecked();
    if(linkageMode && removeNode){
        ui->linkageMode->setChecked(false);
        ui->removeMode->setChecked(false);
    }
}

void Board::on_object_clicked(SharedNodePtr clicked_object){
    if(!first_activated){
        first_activated = clicked_object;
        return;
    }
    second_activated = clicked_object;

    std::string errorMessage;
    if((first_activated && second_activated) && linkageMode){
        if(!Validator::validate(errorMessage, first_activated, second_activated, branchType, diagram->get_diagram())){
            #if DEBUG_MODE>=1
                qDebug() << errorMessage;
                diagram->showDiagram();
            #endif
            QMessageBox::warning(this, "Upozorenje", QString::fromStdString(errorMessage));
        }else{
            diagram->add_branch(first_activated, second_activated, branchType);
        }
    }else if((first_activated && second_activated) && removeMode){
         diagram->remove_branch(first_activated, second_activated, branchType);
    }

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

void Board::add_item(std::shared_ptr<Composition> node) {   //TODO [Nikola] - izmeniti da bude IUMLClassDiagramNode umesto Composition
    CppClass* item = new CppClass(this, node);
    scene->addItem(item);
    diagram->add_node(item);

    connect(item, &CppClass::objectClicked, this, &Board::on_object_clicked);
}


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
            new_node->add_method("method","void", Visibility::Private, MethodKind::Regular, { Argument("bar", "int") });


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
            new_node->add_method("method","void", Visibility::Private, MethodKind::Regular, { Argument("bar", "int") });

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
            new_node->add_method("method","void", Visibility::Private, MethodKind::Regular, { Argument("bar", "int") });


            add_item(new_node);
            break;
        }
    }

}
void Board::add_item(std::shared_ptr<Composition> node) {   //TODO [Nikola] - izmeniti da bude IUMLClassDiagramNode umesto Composition
    CppClass* item = new CppClass(this, node);
    scene->addItem(item);
    //dynamic_cast<CPPStruct*>(item->getClassDiagramNode().get())->parent = item;
    diagram->add_node(item);

    connect(item, &CppClass::add_field_request, this, &Board::on_add_field_requested);
    connect(item, &CppClass::add_method_request, this, &Board::on_add_method_requested);
    connect(item, &CppClass::edit_field_request, this, &Board::on_edit_field_requested);
    connect(item, &CppClass::edit_method_request, this, &Board::on_edit_method_requested);
}

#include "Board.h"
#include "view/cpp_class.h"
#include "model/elements/composition/cpp_struct.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_enum.h"
// ONLY FOR SLEEP
#include <unistd.h>
#include <QGraphicsView>
#include "src/ui/ui_Board.h"
#include "model/elements/type/regular_type.h"
#include "nodefactory.h"
#include "image_paths.h"

Board::Board(QWidget *parent)
    : QWidget(parent), ui(new Ui::Board), diagram(new DiagramGraph())
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    scene = new QGraphicsScene(this);
    ui->board->setScene(scene);
    ui->board->setStyleSheet("background-color: white");
    ui->side_menu->setStyleSheet("background-color: #2c3e50;");
    ui->Linkage->setChecked(false);

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
    connect(ui->Linkage, &QPushButton::clicked, this, &Board::onLinkageToggled);
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

void Board::onLinkageToggled(){
    this->linkageMode = ui->Linkage->isChecked();
    diagram->first_selected_node = nullptr;
    diagram->second_selected_node = nullptr;
}

void Board::ValidateAndLink(SharedNodePtr activator){
    if(diagram->first_selected_node == nullptr){
        diagram->first_selected_node = activator;
        return;
    }
    diagram->second_selected_node = activator;
    //dynamic_cast<CppClass*>
    //qDebug() << "process on " << diagram->first_selected_node->get_uml_class_diagram_node()->get_name() << " ->" << diagram->second_selected_node->get_uml_class_diagram_node()->get_name();

    std::string errorMessage = "/";
    if(!Validator::validateDiagram(errorMessage, diagram->first_selected_node, diagram->second_selected_node, branchType, diagram->get_diagram())){
#if DEBUG_MODE>=1
        qDebug() << errorMessage;
        diagram->showDiagram();
#endif
        diagram->first_selected_node = nullptr;
        diagram->second_selected_node = nullptr;
        return;
    }


    CppClass* f = dynamic_cast<CppClass*>(diagram->first_selected_node);
    CppClass* s = dynamic_cast<CppClass*>(diagram->second_selected_node);

    f->addLineConnection(s, branchType);

    last.first = f;
    last.second = s;
    operations.push_back(last);
    diagram->first_selected_node = nullptr;
    diagram->second_selected_node = nullptr;

}




void Board::onAddClassClicked()     { openNodeFactory(NodeType::Class);  }
void Board::onAddInterfaceClicked() { openNodeFactory(NodeType::Struct); }
void Board::onAddEnumClicked()      { openNodeFactory(NodeType::Enum);   }

<<<<<<< HEAD
void Board::on_add_field_requested(Composition *node, std::shared_ptr<Field> field)
{
    if(node)
    {
        node->add_field(field);
    }

    qDebug() << "Recieved signal add field from: " << node->get_label();
=======
void Board::onUndo(){
    last = operations[operations.size()-3];
    last.first->removeLink(last.second);
    operations.pop_back();
}

void Board::add_item(std::shared_ptr<Composition> node) {   //TODO [Nikola] - izmeniti da bude IUMLClassDiagramNode umesto Composition
    CppClass* item = new CppClass(this, node);
    scene->addItem(item);
    //dynamic_cast<CPPStruct*>(item->getClassDiagramNode().get())->parent = item;
    diagram->add_node(item);
>>>>>>> 4408906 (implemented diamond and cyclic detection. Created removeLink function and fixed GUI glitches)
}

void Board::on_add_method_requested(Composition *node, std::shared_ptr<Method> method)
{
    if(node)
    {
        node->add_method(method);
    }

    qDebug() << "Recieved signal add method from: " << node->get_label();
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

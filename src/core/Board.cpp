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

Board::Board(QWidget *parent)
    : QWidget(parent), ui(new Ui::Board), diagram(new DiagramGraph())
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    scene = new QGraphicsScene(this);
    ui->board->setScene(scene);
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
    qDebug() << "process on " << diagram->first_selected_node->get_uml_class_diagram_node()->get_name() << " ->" << diagram->second_selected_node->get_uml_class_diagram_node()->get_name();

    std::string errorMessage = "/";
    Validator::validateDiagram(errorMessage, diagram->first_selected_node, diagram->second_selected_node, branchType, diagram->get_diagram());
#if DEBUG >=1
    qDebug() << errorMessage;
    diagram->showDiagram();
#endif
    //diagram->first_selected_node->CPPCLASS->add
    diagram->first_selected_node = nullptr;
    diagram->second_selected_node = nullptr;

}


void Board::onAddClassClicked()     { openNodeFactory(NodeType::Class);  }
void Board::onAddInterfaceClicked() { openNodeFactory(NodeType::Struct); }
void Board::onAddEnumClicked()      { openNodeFactory(NodeType::Enum);   }

void Board::add_item(std::shared_ptr<Composition> node) {   //TODO [Nikola] - izmeniti da bude IUMLClassDiagramNode umesto Composition
    CppClass* item = new CppClass(this, node);
    scene->addItem(item);
    //dynamic_cast<CPPStruct*>(item->getClassDiagramNode().get())->parent = item;
    diagram->add_node(item);
}

void Board::openNodeFactory(NodeType node_type) {
    NodeFactory* node_factory = new NodeFactory(node_type);
    connect(node_factory, &NodeFactory::generateClicked, this, &Board::onGenerateClicked);
    node_factory->show();
}

void Board::onGenerateClicked(const QString& class_name, NodeType node_type) {
    switch(node_type) {
    case NodeType::Class: {
        auto new_node = std::make_shared<CPPClass>(class_name);
        new_node->add_field("test1", std::make_shared<RegularType>(RegularType("int")));
        new_node->add_field("test2", std::make_shared<RegularType>(RegularType("bool")));
        new_node->add_field("test3", std::make_shared<RegularType>(RegularType("float")));
        new_node->add_field("test4", std::make_shared<RegularType>(RegularType("double")));
        new_node->add_method("foo", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });

        add_item(new_node);
        break;
    }
    case NodeType::Struct: {
        auto new_node = std::make_shared<CPPStruct>(class_name);
        new_node->add_field("test1", std::make_shared<RegularType>(RegularType("int")));
        new_node->add_field("test2", std::make_shared<RegularType>(RegularType("bool")));
        new_node->add_field("test3", std::make_shared<RegularType>(RegularType("float")));
        new_node->add_field("test4", std::make_shared<RegularType>(RegularType("double")));
        new_node->add_method("foo", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });

        add_item(new_node);
        break;
    }
    case NodeType::Enum: {
        auto new_node = std::make_shared<CPPClass>(class_name);
        new_node->add_field("test1", std::make_shared<RegularType>(RegularType("int")));
        new_node->add_field("test2", std::make_shared<RegularType>(RegularType("bool")));
        new_node->add_field("test3", std::make_shared<RegularType>(RegularType("float")));
        new_node->add_field("test4", std::make_shared<RegularType>(RegularType("double")));
        new_node->add_method("foo", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });

        add_item(new_node);
        break;
    }
    }

}

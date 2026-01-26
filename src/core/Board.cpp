#include "Board.h"
#include "view/cpp_class.h"
#include "model/elements/composition/cpp_struct.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_enum.h"
#include <QGraphicsView>
#include "src/ui/ui_Board.h"
#include "model/elements/type/regular_type.h"
#include "nodefactory.h"

Board::Board(QWidget *parent)
    : QWidget(parent), ui(new Ui::Board), diagram(new DiagramGraph())
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->board->setScene(scene);
    ui->side_menu->setStyleSheet("background-color: #2c3e50;");
    ui->checkBox->setChecked(false);

    // Connect buttons to slots
    connect(ui->add_class, &QPushButton::clicked, this, &Board::onAddClassClicked);
    connect(ui->add_interface, &QPushButton::clicked, this, &Board::onAddInterfaceClicked);
    connect(ui->add_enum, &QPushButton::clicked, this, &Board::onAddEnumClicked);
    connect(ui->radioButton_1, &QPushButton::clicked, this, &Board::onCheckRadioButtonToggled);
    connect(ui->radioButton_2, &QPushButton::clicked, this, &Board::onCheckRadioButtonToggled);
    connect(ui->radioButton_3, &QPushButton::clicked, this, &Board::onCheckRadioButtonToggled);
    connect(ui->checkBox, &QPushButton::clicked, this, &Board::onLinkageToggled);
}

Board::~Board()
{
    delete diagram;
    delete ui;
    delete scene;
}

void Board::onCheckRadioButtonToggled(){
    this->inheritance = ui->radioButton_1->isChecked();
    this->association = ui->radioButton_2->isChecked();
    this->navigation = ui->radioButton_3->isChecked();
}

BranchType determineBranchType(bool inheritance, bool association) {
    if (inheritance) return BranchType::INHERITANCE;
    if (association) return BranchType::ASSOCIATION;
    return BranchType::NAVIGATION;
}

void Board::ValidateAndLink(SharedNodePtr activator){
    if(diagram->first_selected_node == nullptr){
        diagram->first_selected_node = activator;
        return;
    }
    diagram->second_selected_node = activator;

    BranchType type = determineBranchType(this->inheritance, this->association);
    // dodati if i raditi
    validator->checkLinkage(diagram->first_selected_node, diagram->second_selected_node, type);
    diagram->add_branch(diagram->first_selected_node, diagram->second_selected_node, type);
    // createConnection ?

    validator->checkLinkage(diagram->first_selected_node, diagram->second_selected_node, type);

    diagram->add_branch(diagram->first_selected_node, diagram->second_selected_node, type);
    // createConnection ?
    diagram->first_selected_node = nullptr;
    diagram->second_selected_node = nullptr;
}

void Board::onLinkageToggled(){
    this->linkageMode = ui->checkBox->isChecked();
}

void Board::on_add_element_requested(IUMLClassDiagramNode *node, IClassElement* element)
{
    qDebug() << "Recieved signal add element signal from: " << node->get_label();
}

void Board::on_edit_element_requested(IUMLClassDiagramNode *node, IClassElement* element)
{
    qDebug() << "Recieved signal edit element signal from: " << node->get_label();
}


void Board::onAddClassClicked()
{
    NodeFactory* node_factory = new NodeFactory(NodeType::Class, [this](const QString class_name) {

        auto new_node = std::make_shared<CPPClass>(class_name);
        // new_node->add_field("test1", std::make_shared<RegularType>(RegularType("int")));
        // new_node->add_field("test2", std::make_shared<RegularType>(RegularType("bool")));
        // new_node->add_field("test3", std::make_shared<RegularType>(RegularType("float")));
        // new_node->add_field("test4", std::make_shared<RegularType>(RegularType("double")));
        // new_node->add_method("foo", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });


        new_node->add_field("test1", "tip1", Visibility::Protected);
        new_node->add_field("test2", "tip2", Visibility::Private);
        new_node->add_field("test3", "tip3");
        new_node->add_method("method", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });


        add_item(new_node);
    });
    node_factory->show();
}

void Board::onAddInterfaceClicked()
{
    NodeFactory* node_factory = new NodeFactory(NodeType::Class, [this](const QString class_name) {

        auto new_node = std::make_shared<CPPStruct>(class_name);
        // new_node->add_field("test1", std::make_shared<RegularType>(RegularType("int")));
        // new_node->add_field("test2", std::make_shared<RegularType>(RegularType("bool")));
        // new_node->add_field("test3", std::make_shared<RegularType>(RegularType("float")));
        // new_node->add_field("test4", std::make_shared<RegularType>(RegularType("double")));
        // new_node->add_method("foo", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });


        new_node->add_field("test1", "tip1", Visibility::Protected);
        new_node->add_field("test2", "tip2", Visibility::Private);
        new_node->add_field("test3", "tip3");
        new_node->add_method("method", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });

        add_item(new_node);
    });
    node_factory->show();
}

void Board::onAddEnumClicked()
{
    NodeFactory* node_factory = new NodeFactory(NodeType::Class, [this](const QString class_name) {

        auto new_node = std::make_shared<CPPClass>(class_name);
        // new_node->add_field("test1", std::make_shared<RegularType>(RegularType("int")));
        // new_node->add_field("test2", std::make_shared<RegularType>(RegularType("bool")));
        // new_node->add_field("test3", std::make_shared<RegularType>(RegularType("float")));
        // new_node->add_field("test4", std::make_shared<RegularType>(RegularType("double")));
        // new_node->add_method("foo", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });

        new_node->add_field("test1", "tip1", Visibility::Protected);
        new_node->add_field("test2", "tip2", Visibility::Private);
        new_node->add_field("test3", "tip3");
        new_node->add_method("method", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });


        add_item(new_node);
    });
    node_factory->show();
}

void Board::add_item(std::shared_ptr<Composition> node) {   //TODO [Nikola] - izmeniti da bude IUMLClassDiagramNode umesto Composition
    CppClass* item = new CppClass(this, node);
    scene->addItem(item);
    //dynamic_cast<CPPStruct*>(item->getClassDiagramNode().get())->parent = item;
    diagram->add_node(item);

    connect(item, &CppClass::add_element_request, this, &Board::on_add_element_requested);
    connect(item, &CppClass::edit_element_request, this, &Board::on_edit_element_requested);
}

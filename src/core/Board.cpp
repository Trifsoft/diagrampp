#include "Board.h"
#include "view/cpp_class.h"
#include "model/elements/composition/cpp_struct.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_enum.h"
#include <QGraphicsView>
#include "src/ui/ui_Board.h"
#include "model/elements/type/regular_type.h"

Board::Board(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Board)
{
    ui->setupUi(this);
    diagram = new DiagramGraph();

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

void Board::ValidateAndLink(std::shared_ptr<IUMLClassDiagramNode> activator){
    if(diagram->firstNode == nullptr){
        diagram->firstNode = activator;
        return;
    }
    diagram->secondNode = activator;

    BranchType type = determineBranchType(this->inheritance, this->association);
    // dodati if i raditi
    validator->checkLinkage(diagram->firstNode, diagram->secondNode, type);
    diagram->addLink(diagram->firstNode, diagram->secondNode, type);
    // createConnection ?
    diagram->firstNode = nullptr;
    diagram->secondNode = nullptr;
}

void Board::onLinkageToggled(){
    this->linkageMode = ui->checkBox->isChecked();
}

void Board::onAddClassClicked()
{
    CppClass* item = new CppClass(this,
        std::make_shared<CPPClass>(
            CPPClass("Class")
            )
        );
    item->change_composition([](std::shared_ptr<Composition> c) {
        c->add_field("test1", std::make_shared<RegularType>(RegularType("int")));
        c->add_field("test2", std::make_shared<RegularType>(RegularType("bool")));
        c->add_field("test3", std::make_shared<RegularType>(RegularType("float")));
        c->add_field("test4", std::make_shared<RegularType>(RegularType("double")));

        c->add_method("foo", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });
    });

    scene->addItem(item);
    dynamic_cast<CPPClass*>(item->getClassDiagramNode().get())->parent = item;
    diagram->addNode(item->getClassDiagramNode());
}

void Board::onAddInterfaceClicked()
{
    CppClass* item = new CppClass(this,
        std::make_shared<CPPStruct>(
            CPPStruct("Interface")
        )
    );
    item->change_composition([](std::shared_ptr<Composition> c) {
        c->add_field("test1", std::make_shared<RegularType>(RegularType("int")));
        c->add_field("test2", std::make_shared<RegularType>(RegularType("bool")));
        c->add_field("test3", std::make_shared<RegularType>(RegularType("float")));
        c->add_field("test4", std::make_shared<RegularType>(RegularType("double")));
        c->add_method("foo", std::make_shared<RegularType>("void"), Visibility::Private, MethodType::Regular, { Description("bar", std::make_shared<RegularType>("int")) });
    });
    scene->addItem(item);
    dynamic_cast<CPPStruct*>(item->getClassDiagramNode().get())->parent = item;
    diagram->addNode(item->getClassDiagramNode());
}

void Board::onAddEnumClicked()
{
    CppClass* item = new CppClass(this,
        std::make_shared<CPPStruct>(
            CPPStruct("Enum")
        )
    );
    dynamic_cast<CPPEnum*>(item->getClassDiagramNode().get())->parent = item;
    diagram->addNode(item->getClassDiagramNode());
}

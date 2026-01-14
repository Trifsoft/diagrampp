#include "Board.h"
#include "view/cpp_class.h"
#include "model/elements/composition/cpp_struct.h"
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

    // Connect buttons to slots
    connect(ui->add_class, &QPushButton::clicked, this, &Board::onAddClassClicked);
    connect(ui->add_interface, &QPushButton::clicked, this, &Board::onAddInterfaceClicked);
    connect(ui->add_enum, &QPushButton::clicked, this, &Board::onAddEnumClicked);
}

Board::~Board()
{
    delete diagram;
    delete ui;
    delete scene;
}

void Board::onAddClassClicked()
{
    CppClass* item = new CppClass(
        std::make_shared<CPPStruct>(
            CPPStruct("Class")
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
    diagram->addNode(item->getComposition().get());
}

void Board::onAddInterfaceClicked()
{
    CppClass* item = new CppClass(
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
    diagram->addNode(item->getComposition().get());
}

void Board::onAddEnumClicked()
{

    CppClass* item = new CppClass(
        std::make_shared<CPPStruct>(
            CPPStruct("Enum")
        )
    );
    diagram->addNode(item->getComposition().get());
}

#include "project.h"
#include "view/cpp_class_view.h"
#include "model/elements/composition/cpp_struct.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_enum.h"
#include <QGraphicsView>
#include "src/ui/ui_project.h"
#include "nodefactory.h"

Project::Project(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Project)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->board->setScene(scene);

    // Connect buttons to slots
    connect(ui->add_class, &QPushButton::clicked, this, &Project::onAddClassClicked);
    connect(ui->add_interface, &QPushButton::clicked, this, &Project::onAddInterfaceClicked);
    connect(ui->add_enum, &QPushButton::clicked, this, &Project::onAddEnumClicked);
}

Project::~Project()
{
    delete ui;
    delete scene;
}

void Project::onAddClassClicked()
{
    NodeFactory* node_factory = new NodeFactory(NodeType::Class, [this](const QString class_name) {
        CppClassView* item = new CppClassView(
            std::make_shared<CPPClass>(class_name)
        );
        scene->addItem(item);
    });
    node_factory->show();
}

void Project::onAddInterfaceClicked()
{
    NodeFactory* node_factory = new NodeFactory(NodeType::Struct, [this](const QString class_name) {
        CppClassView* item = new CppClassView(
            std::make_shared<CPPStruct>(class_name)
        );
        scene->addItem(item);
    });
    node_factory->show();
}

void Project::onAddEnumClicked()
{
    NodeFactory* node_factory = new NodeFactory(NodeType::Enum, [this](const QString class_name) {
        //TODO zameniti CppClassView ovde sa novim nekim tipom "CppEnumView"
        CppClassView* item = new CppClassView(
            std::make_shared<CPPStruct>(class_name)
        );
        scene->addItem(item);
    });
    node_factory->show();
}

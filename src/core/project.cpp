#include "project.h"
#include "view/cpp_class.h"
#include "model/elements/composition/cpp_struct.h"
#include <QGraphicsView>
#include "src/ui/ui_project.h"
#include "model/elements/type/regular_type.h"
#include "commandManager/command_manager.h"
#include "commandManager/add_node_command.h"
#include "model/elements/composition/cpp_class.h"

Project::Project(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Project)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->board->setScene(scene);
    
    // Inicijalizacija CommandManagera sa referencom na scenu
    commandManager = std::make_unique<CommandManager>(scene);

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
    
    //  Kako treba da izgledaju komande
    DiagramGraph* graph = new DiagramGraph();
    IUMLClassDiagramNode* node = new CPPClass("Ime");
    auto command = std::make_shared<AddNodeCommand>(scene, graph, node);
    commandManager->execute(command);
}

void Project::onAddInterfaceClicked()
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
}

void Project::onAddEnumClicked()
{

    CppClass* item = new CppClass(
        std::make_shared<CPPStruct>(
            CPPStruct("Enum")
        )
    );
}

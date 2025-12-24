#include "project.h"
#include "view/cpp_class.h"
#include "model/elements/composition/cpp_struct.h"
#include <QGraphicsView>
#include "src/ui/ui_project.h"
#include "model/elements/type/regular_type.h"
#include "commandManager/command_manager.h"
#include "commandManager/add_node_command.h"
#include "commandManager/remove_node_command.h"
#include "model/elements/composition/cpp_class.h"

Project::Project(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Project)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->board->setScene(scene);
    
    // Inicijalizacija CommandManagera
    m_command_manager = std::make_unique<CommandManager>();
    m_graph = std::make_unique<DiagramGraph>();

    // Connect buttons to slots
    connect(ui->add_class, &QPushButton::clicked, this, &Project::onAddClassClicked);
    connect(ui->add_interface, &QPushButton::clicked, this, &Project::onAddInterfaceClicked);
    connect(ui->add_enum, &QPushButton::clicked, this, &Project::onAddEnumClicked);
<<<<<<< HEAD

    setup_actions();
=======
    connect(ui->generate_project, &QPushButton::clicked, this, &Project::onGenerateProjectClicked);
>>>>>>> 126b8c5 (Add GenerateButton)
}

Project::~Project()
{
    delete ui;
    delete scene;
}
//*******************************************************************************************************************

void Project::setup_actions()
{
    m_undo_action = new QAction("Undo", this);
    m_undo_action->setShortcut(QKeySequence::Undo);  // Ctrl+Z
    connect(m_undo_action, &QAction::triggered, this, &Project::onUndo);
    addAction(m_undo_action);

    m_redo_action = new QAction("Redo", this);
    m_redo_action->setShortcut(QKeySequence::Redo);  // Ctrl+Shift+Z
    connect(m_redo_action, &QAction::triggered, this, &Project::onRedo);
    addAction(m_redo_action);

}
void Project::onUndo()
{
    m_command_manager->undo();
}

void Project::onRedo()
{
    m_command_manager->redo();
}

//*******************************************************************************************************************

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
    scene->addItem(item);
    
    /* Kako treba da izgleda kreiranje i pozivanje komande
    auto node = std::make_shared<CPPClass>("Ime");
    auto command = std::make_shared<AddNodeCommand>(m_graph.get(), node);
    m_command_manager->execute(command);
    */
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

void Project::onGenerateProjectClicked()
{

}


// imeProjekta
/*
 * src
 *     getnaem .cpp
 * include
 *      .hpp
 */

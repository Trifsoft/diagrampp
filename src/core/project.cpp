#include "project.h"
#include "cpp_class_item.h"
#include <QGraphicsView>
#include "src/ui/ui_project.h"

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
    connect(ui->generate_project, &QPushButton::clicked, this, &Project::onGenerateProjectClicked);
}

Project::~Project()
{
    delete ui;
    delete scene;
}

void Project::onAddClassClicked()
{
    CppClassItem* item = new CppClassItem("Class", CppClassItem::ClassType);
    scene->addItem(item);
}

void Project::onAddInterfaceClicked()
{
    CppClassItem* item = new CppClassItem("Interface", CppClassItem::InterfaceType);
    scene->addItem(item);
}

void Project::onAddEnumClicked()
{
    CppClassItem* item = new CppClassItem("Enum", CppClassItem::EnumType);
    scene->addItem(item);
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

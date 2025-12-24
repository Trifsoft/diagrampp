#include "project.h"
#include "cpp_class_item.h"
#include <QGraphicsView>
#include <filesystem>
#include <vector>
#include <iostream>
#include <fstream>
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


void Project::generateProjectFS(std::string& file_path, std::string& project_name)
{
    namespace fs = std::filesystem;

    const std::string dirs[] = {"include", "src/core", "src/ui"};
    projectPath = file_path + project_name + "/";
    try{
        fs::create_directory(projectPath);
        for(int i = 0; i < std::size(dirs); i++){
            dirPaths[i] = projectPath + dirs[i];
            fs::create_directory(dirPaths[i]);
        }
    }catch(const fs::filesystem_error &e){
        std::cerr << e.what() << '\n';
    }

}


void Project::generateProjectFiles()
{
    std::ofstream CMakeLists(projectPath + "CMakeLists.txt");
    //writeCMake(CMakeLists);
    CMakeLists.close();

    // Treba da se dobije vektor objekata sa metodom getName() da bi mogli da napravimo fajlove.
}


void writeCMake(std::ofstream& CMakeLists)
{
    // Isti vektor je potreban kao gore
}


// imeProjekta
/*
 * src
 *     getnaem .cpp
 * include
 *      .hpp
 */

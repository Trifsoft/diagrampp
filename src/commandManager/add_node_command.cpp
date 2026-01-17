#include "commandManager/add_node_command.h"
#include "model/base/uml_class_diagram_node.h"
#include <QDebug>

AddNodeCommand::AddNodeCommand(QGraphicsScene* scene, DiagramGraph* graph, IUMLClassDiagramNode* node)
    : scene(scene), graph(graph), node(node), nodeAdded(false)
{
}

void AddNodeCommand::execute()
{
    if (!nodeAdded && scene && node) {
        // scene->addItem(node);  // Dodaj node na scenu
        nodeAdded = true;
        qDebug() << "Command EXECUTE: Node dodat na scenu";
    }
}

void AddNodeCommand::undo()
{
    if (nodeAdded && scene && node) {
        // scene->removeItem(node);  // Ukloni node sa scene
        nodeAdded = false;
        qDebug() << "Command UNDO: Node uklonjen sa scene";
    }
}

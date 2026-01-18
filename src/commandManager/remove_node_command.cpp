#include "commandManager/remove_node_command.h"
#include "model/base/uml_class_diagram_node.h"
#include <QDebug>

RemoveNodeCommand::RemoveNodeCommand(DiagramGraph* graph, std::shared_ptr<IUMLClassDiagramNode> node)
    :m_graph(graph), m_node(node), m_node_removed(false)
{
}

void RemoveNodeCommand::execute()
{
    if (!m_node_removed && m_node) {
        m_graph->addNode(m_node);
        m_node_removed = true;
        qDebug() << "Command EXECUTE: Node uklonjen sa scene";
    }
}

void RemoveNodeCommand::undo()
{
    if (m_node_removed && m_node) {
        // graph->removeNode();  // Ukloni node sa scene
        m_node_removed = false;
        qDebug() << "Command UNDO: Node uklonjen sa scene";
    }
}

#include "commandManager/remove_node_command.h"
#include "model/base/uml_class_diagram_node.h"
#include <QDebug>

RemoveNodeCommand::RemoveNodeCommand(DiagramGraph* graph, SharedNodePtr node)
    :m_graph(graph), m_node(node), m_node_removed(false)
{
}

void RemoveNodeCommand::execute()
{
    if (!m_node_removed && m_node && m_graph) {
        m_graph->remove_node(m_node);  //uklanja node
        m_node_removed = true;
        qDebug() << "Command EXECUTE: Node uklonjen sa scene";
    }
}

void RemoveNodeCommand::undo()
{
    if (m_node_removed && m_node && m_graph) {
        m_graph->add_node(m_node);  //vraca node nazad
        m_node_removed = false;
        qDebug() << "Command UNDO: Node vracen na scenu";
    }
}

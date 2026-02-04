#include "commandManager/add_node_command.h"
#include <QDebug>

AddNodeCommand::AddNodeCommand(DiagramGraph* graph,
                               SharedNodePtr node)
    :m_graph(graph), m_node(node), m_node_added(false)
{
}

void AddNodeCommand::execute()
{
    if (!m_node_added && m_node && m_graph) {
        m_graph->add_node(m_node);
        m_node_added = true;
        qDebug() << "Command EXECUTE: Node dodat na scenu";
    }
}

void AddNodeCommand::undo()
{
    if (m_node_added && m_node && m_graph) {
        m_graph->remove_node(m_node);  // Implementirano: uklanja node sa scene
        m_node_added = false;
        qDebug() << "Command UNDO: Node uklonjen sa scene";
    }
}

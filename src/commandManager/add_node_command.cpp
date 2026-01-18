#include "commandManager/add_node_command.h"
#include "model/base/uml_class_diagram_node.h"
#include <QDebug>

AddNodeCommand::AddNodeCommand(DiagramGraph* graph,
                               std::shared_ptr<IUMLClassDiagramNode> node)
    :m_graph(graph), m_node(node), m_node_added(false)
{
}

void AddNodeCommand::execute()
{
    if (!m_node_added && m_node && m_graph) {
        m_graph->addNode(m_node);
        m_node_added = true;
        qDebug() << "Command EXECUTE: Node dodat na scenu";
    }
}

void AddNodeCommand::undo()
{
    if (m_node_added && m_node && m_graph) {
        // m_graph->removeNode(m_node);
        m_node_added = false;
        qDebug() << "Command UNDO: Node uklonjen sa scene";
    }
}

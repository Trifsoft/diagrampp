#include "commandManager/add_node_command.h"
#include <QDebug>

AddNodeCommand::AddNodeCommand(QString name, NodeType nodeType)
    : QObject(nullptr), mName(name), mNodeType(nodeType), m_node_added(false) {}

bool AddNodeCommand::execute()
{
    emit addNodeRequested(mName, mNodeType);
    return true;
    // if (!m_node_added && m_node && m_graph) {
    //     m_graph->add_node(m_node);
    //     m_node_added = true;
    //     qDebug() << "Command EXECUTE: Node dodat na scenu";
    //     return true;
    // }
    // return false;
}

bool AddNodeCommand::undo()
{
    emit removeNodeRequested(mName, mNodeType);
    return true;
    // if (m_node_added && m_node && m_graph) {
    //     m_graph->remove_node(m_node);
    //     m_node_added = false;
    //     qDebug() << "Command UNDO: Node uklonjen sa scene";
    //     return true;
    // }
    // return false;
}

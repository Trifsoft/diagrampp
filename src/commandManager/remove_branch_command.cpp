#include "commandManager/remove_branch_command.h"
#include <QDebug>

RemoveBranchCommand::RemoveBranchCommand(DiagramGraph* graph,
                                     SharedNodePtr from,
                                     SharedNodePtr to,
                                     BranchType branch_type)
    : m_graph(graph), m_from(from), m_to(to),
      m_branch_type(branch_type), m_branch_removed(false)
{
}

bool RemoveBranchCommand::execute()
{
    if (!m_branch_removed && m_from && m_to && m_graph) {
        m_graph->remove_branch(m_from, m_to, m_branch_type);
        m_branch_removed = true;
        qDebug() << "Command EXECUTE: Branch uklonjen sa scene";
        return true;
    }
    return false;
}

bool RemoveBranchCommand::undo()
{
    if (m_branch_removed && m_from && m_to && m_graph) {
        m_graph->add_branch(m_from, m_to, m_branch_type);
        m_branch_removed = false;
        qDebug() << "Command UNDO: Branch vracen na scenu";
        return true;
    }
    return false;
}

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
    m_graph->removeBranch(m_from, m_to, m_branch_type);
    return true;
}

bool RemoveBranchCommand::undo()
{
    m_graph->addBranch(m_from, m_to, m_branch_type);
    return true;
}

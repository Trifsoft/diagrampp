#include "commandManager/remove_branch_command.h"
#include <QDebug>


RemoveBranchCommand::RemoveBranchCommand(SharedNodePtr from,
                                   SharedNodePtr to,
                                   BranchType branch_type)
    : QObject(nullptr), m_from(from), m_to(to),
    m_branch_type(branch_type)
{
}

bool RemoveBranchCommand::execute()
{
    emit removeBranchRequested(m_from, m_to, m_branch_type);
    return true;
}

bool RemoveBranchCommand::undo()
{
    emit addBranchRequested(m_from, m_to, m_branch_type);
    return false;
}

#include "commandManager/add_branch_command.h"
#include <QDebug>


AddBranchCommand::AddBranchCommand(SharedNodePtr from,
                               SharedNodePtr to,
                               BranchType branch_type)
    : QObject(nullptr), m_from(from), m_to(to),
      m_branch_type(branch_type)
{
}

bool AddBranchCommand::execute()
{
    emit addBranchRequested(m_from, m_to, m_branch_type);
    return true;
}

bool AddBranchCommand::undo()
{
    emit removeBranchRequested(m_from, m_to, m_branch_type);
    return false;
}

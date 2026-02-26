#ifndef ADD_BRANCH_COMMAND_H
#define ADD_BRANCH_COMMAND_H

#include "command.h"
#include <shared_node_ptr.h>
#include <model/base/branches.h>

class AddBranchCommand : public QObject, public Command
{
    Q_OBJECT
public:
    AddBranchCommand(NodePtr from,
                   NodePtr to,
                   BranchType branch_type);
    
    bool execute() override;
    bool undo() override;
signals:
    void addBranchRequested(NodePtr, NodePtr, BranchType);
    void removeBranchRequested(NodePtr, NodePtr, BranchType);
private:
    NodePtr m_from;
    NodePtr m_to;
    BranchType m_branch_type;
};

#endif // ADD_BRANCH_COMMAND_H

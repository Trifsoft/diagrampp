#ifndef ADD_BRANCH_COMMAND_H
#define ADD_BRANCH_COMMAND_H

#include "command.h"
#include <shared_node_ptr.h>
#include <model/base/branches.h>

class IUMLClassDiagramNode;

class AddBranchCommand : public QObject, public Command
{
    Q_OBJECT
public:
    AddBranchCommand(SharedNodePtr from,
                   SharedNodePtr to,
                   BranchType branch_type);
    
    bool execute() override;
    bool undo() override;
signals:
    void addBranchRequested(SharedNodePtr, SharedNodePtr, BranchType);
    void removeBranchRequested(SharedNodePtr, SharedNodePtr, BranchType);
private:
    SharedNodePtr m_from;
    SharedNodePtr m_to;
    BranchType m_branch_type;
};

#endif // ADD_BRANCH_COMMAND_H

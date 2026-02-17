#ifndef REMOVE_NODE_COMMAND_H
#define REMOVE_NODE_COMMAND_H

#include "command.h"
#include <QString>
#include <node_type.h>
#include <QObject>
#include <shared_node_ptr.h>
#include <model/base/branches.h>

class RemoveNodeCommand : public QObject, public Command
{
    Q_OBJECT
public:
    RemoveNodeCommand(SharedNodePtr, const QList<std::tuple<SharedNodePtr, SharedNodePtr, BranchType>>&);

    bool execute() override;
    bool undo() override;
signals:
    void addNodeRequested(SharedNodePtr);
    void removeNodeRequested(SharedNodePtr);
    void addBranchRequested(SharedNodePtr, SharedNodePtr, BranchType);
    void removeBranchRequested(SharedNodePtr, SharedNodePtr, BranchType);
private:
    SharedNodePtr mNode;
    QList<std::tuple<SharedNodePtr, SharedNodePtr, BranchType>> mBranches;
    bool m_node_added;
};

#endif // REMOVE_NODE_COMMAND_H

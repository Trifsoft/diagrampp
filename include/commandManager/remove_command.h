#ifndef REMOVE_COMMAND_H
#define REMOVE_COMMAND_H

#include "command.h"
#include <QString>
#include <node_type.h>
#include <QObject>
#include <shared_node_ptr.h>
#include <model/base/branches.h>

class RemoveCommand : public QObject, public Command
{
    Q_OBJECT
public:
    RemoveCommand(const QList<NodePtr>,
                  const QList<std::tuple<NodePtr, NodePtr, BranchType>>);

    bool execute() override;
    bool undo() override;
signals:
    void addNodeRequested(NodePtr);
    void removeNodeRequested(NodePtr);
    void addBranchRequested(NodePtr, NodePtr, BranchType);
    void removeBranchRequested(NodePtr, NodePtr, BranchType);
private:
    QList<NodePtr> mNodes;
    QList<std::tuple<NodePtr, NodePtr, BranchType>> mBranches;
    bool m_node_added;
};

#endif // REMOVE_NODE_COMMAND_H

#include "commandManager/remove_node_command.h"
#include <QDebug>

RemoveNodeCommand::RemoveNodeCommand(SharedNodePtr node,
                                     const QList<std::tuple<SharedNodePtr, SharedNodePtr, BranchType>>& branches)
    : QObject(nullptr), mNode(node), mBranches(branches) {}

bool RemoveNodeCommand::execute()
{
    for(auto& [iFrom, iTo, iBranch] : mBranches) {
        emit removeBranchRequested(iFrom, iTo, iBranch);
    }
    emit removeNodeRequested(mNode);
    return true;
}

bool RemoveNodeCommand::undo()
{
    emit addNodeRequested(mNode);
    for(auto& [iFrom, iTo, iBranch] : mBranches) {
        emit addBranchRequested(iFrom, iTo, iBranch);
    }
    return true;
}

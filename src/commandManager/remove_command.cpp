#include "commandManager/remove_command.h"
#include <QDebug>

RemoveCommand::RemoveCommand(const QList<NodePtr> nodes,
                            const QList<std::tuple<NodePtr, NodePtr, BranchType>> branches)
    : QObject(nullptr), mNodes(nodes), mBranches(branches) {}

bool RemoveCommand::execute()
{
    for(auto& [iFrom, iTo, iBranch] : mBranches) {
        emit removeBranchRequested(iFrom, iTo, iBranch);
    }
    for(auto& node : mNodes) {
        emit removeNodeRequested(node);
    }
    return true;
}

bool RemoveCommand::undo()
{
    for(auto& node : mNodes) {
        emit addNodeRequested(node);
    }
    for(auto& [iFrom, iTo, iBranch] : mBranches) {
        emit addBranchRequested(iFrom, iTo, iBranch);
    }
    return true;
}

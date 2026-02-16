#include "commandManager/remove_node_command.h"
#include <QDebug>

RemoveNodeCommand::RemoveNodeCommand(SharedNodePtr node)
    : QObject(nullptr), mNode(node) {}

bool RemoveNodeCommand::execute()
{
    emit removeNodeRequested(mNode);
    return true;
}

bool RemoveNodeCommand::undo()
{
    emit addNodeRequested(mNode);
    return true;
}

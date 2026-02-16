#include "commandManager/add_node_command.h"
#include <QDebug>

AddNodeCommand::AddNodeCommand(SharedNodePtr node)
    : QObject(nullptr), mNode(node) {}

bool AddNodeCommand::execute()
{
    emit addNodeRequested(mNode);
    return true;
}

bool AddNodeCommand::undo()
{
    emit removeNodeRequested(mNode);
    return true;
}

#ifndef REMOVE_NODE_COMMAND_H
#define REMOVE_NODE_COMMAND_H

#include "command.h"
#include <QString>
#include <node_type.h>
#include <QObject>
#include<shared_node_ptr.h>

class RemoveNodeCommand : public QObject, public Command
{
    Q_OBJECT
public:
    RemoveNodeCommand(SharedNodePtr);

    bool execute() override;
    bool undo() override;
signals:
    void addNodeRequested(SharedNodePtr);
    void removeNodeRequested(SharedNodePtr);
private:
    SharedNodePtr mNode;
    bool m_node_added;
};

#endif // REMOVE_NODE_COMMAND_H

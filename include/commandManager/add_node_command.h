#ifndef ADD_NODE_COMMAND_H
#define ADD_NODE_COMMAND_H

#include "command.h"
#include <QString>
#include <node_type.h>
#include <QObject>
#include<shared_node_ptr.h>

class AddNodeCommand : public QObject, public Command
{
    Q_OBJECT
public:
    AddNodeCommand(SharedNodePtr);
    
    bool execute() override;
    bool undo() override;
signals:
    void addNodeRequested(SharedNodePtr);
    void removeNodeRequested(SharedNodePtr);
private:
    SharedNodePtr mNode;
    bool m_node_added;
};

#endif // ADD_NODE_COMMAND_H

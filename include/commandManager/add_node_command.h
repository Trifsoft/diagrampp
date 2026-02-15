#ifndef ADD_NODE_COMMAND_H
#define ADD_NODE_COMMAND_H

#include "command.h"
#include <QString>
#include <node_type.h>
#include <QObject>

class IUMLClassDiagramNode;

class AddNodeCommand : public QObject, public Command
{
    Q_OBJECT
public:
    AddNodeCommand(QString name, NodeType);
    
    bool execute() override;
    bool undo() override;
signals:
    void addNodeRequested(QString, NodeType);
    void removeNodeRequested(QString, NodeType);
private:
    QString mName;
    NodeType mNodeType;
    bool m_node_added;
};

#endif // ADD_NODE_COMMAND_H

#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include <QVector>
#include "command.h"
#include <shared_node_ptr.h>

#include <commandManager/add_node_command.h>
#include <commandManager/add_branch_command.h>

class CommandManager: public QObject
{
    Q_OBJECT
public:
    CommandManager(QObject* parent = nullptr);
    
    void execute(std::shared_ptr<Command> command);
    void undo();
    void redo();
    
    bool can_undo() const;
    bool can_redo() const;
signals:
    void createNodeCommandAdded(std::shared_ptr<AddNodeCommand>);
    void createBranchCommandAdded(std::shared_ptr<AddBranchCommand>);
public slots:
    void addCreateNodeCommand(const QString&, NodeType);
    void addCreateBranchCommand(SharedNodePtr, SharedNodePtr, BranchType);
private:
    QVector<std::shared_ptr<Command>> m_undo_stack;
    QVector<std::shared_ptr<Command>> m_redo_stack;
    void addCommand(std::shared_ptr<Command> command);
};

#endif // COMMAND_MANAGER_H

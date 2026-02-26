#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include <QVector>
#include "command.h"
#include <shared_node_ptr.h>
#include <view/cpp_class_view.h>

#include <commandManager/add_node_command.h>
#include <commandManager/remove_command.h>
#include <commandManager/add_branch_command.h>
#include <commandManager/move_node_command.h>

class CommandManager: public QObject
{
    Q_OBJECT
public:
    CommandManager(QObject* parent = nullptr);
    ~CommandManager();
    
    void execute(std::shared_ptr<Command> command);
    void undo();
    void redo();
    
    bool can_undo() const;
    bool can_redo() const;
signals:
    void createNodeCommandAdded(std::shared_ptr<AddNodeCommand>);
    //void removeNodeCommandAdded(std::shared_ptr<RemoveNodeCommand>);
    void createBranchCommandAdded(std::shared_ptr<AddBranchCommand>);
    //void removeBranchCommandAdded(std::shared_ptr<RemoveBranchCommand>);
    void removeCommandAdded(std::shared_ptr<RemoveCommand>);
public slots:
    void addCreateNodeCommand(NodePtr);
    //void addRemoveNodeCommand(SharedNodePtr, const QList<std::tuple<SharedNodePtr, SharedNodePtr, BranchType>>&);
    void addCreateBranchCommand(NodePtr, NodePtr, BranchType);
    //void addRemoveBranchCommand(SharedNodePtr, SharedNodePtr, BranchType);
    void addRemoveCommand(const QList<NodePtr>&,
                          const QList<std::tuple<NodePtr, NodePtr, BranchType>>&);

    void addMoveNodeCommand(CppClassView*, QPointF);
private:
    QVector<std::shared_ptr<Command>> m_undo_stack;
    QVector<std::shared_ptr<Command>> m_redo_stack;
    void addCommand(std::shared_ptr<Command> command);
};

#endif // COMMAND_MANAGER_H

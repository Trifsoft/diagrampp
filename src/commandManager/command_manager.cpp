#include "commandManager/command_manager.h"
#include <QDebug>

CommandManager::CommandManager(QObject* parent): QObject(parent) {}

CommandManager::~CommandManager()
{
    qDebug() << "Deleted command manager";
};

void CommandManager::execute(std::shared_ptr<Command> command)
{
    if (command && command->execute()) {
        addCommand(command);
    }
}

void CommandManager::undo()
{
    if (can_undo()) {
        auto command = m_undo_stack.back();
        m_undo_stack.pop_back();
        command->undo();
        m_redo_stack.push_back(command);
    }
}

void CommandManager::redo()
{
    if (can_redo()) {
        auto command = m_redo_stack.back();
        m_redo_stack.pop_back();
        m_undo_stack.push_back(command);
        command->execute();
    }
}

bool CommandManager::can_undo() const
{
    return !m_undo_stack.empty();
}

bool CommandManager::can_redo() const
{
    return !m_redo_stack.empty();
}

void CommandManager::addCommand(std::shared_ptr<Command> command) {
    m_undo_stack.push_back(command);
    m_redo_stack.clear();
}
void CommandManager::addCreateNodeCommand(SharedNodePtr node)
{
    auto cmd = std::make_shared<AddNodeCommand>(node);
    addCommand(cmd);
    emit createNodeCommandAdded(cmd);
}

void CommandManager::addRemoveNodeCommand(SharedNodePtr node,
                                          const QList<std::tuple<SharedNodePtr, SharedNodePtr, BranchType>>& branches)
{
    auto cmd = std::make_shared<RemoveNodeCommand>(node, branches);
    addCommand(cmd);
    emit removeNodeCommandAdded(cmd);
}

void CommandManager::addCreateBranchCommand(SharedNodePtr from, SharedNodePtr to, BranchType branchType)
{
    auto cmd = std::make_shared<AddBranchCommand>(from, to, branchType);
    addCommand(cmd);
    emit createBranchCommandAdded(cmd);
}

void CommandManager::addRemoveBranchCommand(SharedNodePtr from, SharedNodePtr to, BranchType branchType)
{
    auto cmd = std::make_shared<RemoveBranchCommand>(from, to, branchType);
    addCommand(cmd);
    emit removeBranchCommandAdded(cmd);
}

void CommandManager::addMoveNodeCommand(CppClassView* item, QPointF delta)
{
    auto cmd = std::make_shared<MoveNode>(item, delta);
    addCommand(cmd);
}

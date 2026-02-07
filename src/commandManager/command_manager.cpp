#include "commandManager/command_manager.h"
#include <QDebug>

void CommandManager::execute(std::shared_ptr<Command> command)
{
    if (command && command->execute()) {
        m_undo_stack.push_back(command);
        m_redo_stack.clear();
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

#include "commandManager/command_manager.h"

CommandManager::CommandManager(QGraphicsScene* scene)
    : scene(scene)
{
}

void CommandManager::execute(std::shared_ptr<Command> command)
{
    if (command) {
        command->execute();
        undoStack.push_back(command);
        redoStack.clear(); // Clear redo stack after new command
    }
}

void CommandManager::undo()
{
    if (canUndo()) {
        auto command = undoStack.back();
        undoStack.pop_back();
        redoStack.push_back(command);
        // Trebate implementirati undo u Command klasi
    }
}

void CommandManager::redo()
{
    if (canRedo()) {
        auto command = redoStack.back();
        redoStack.pop_back();
        undoStack.push_back(command);
        command->execute();
    }
}

bool CommandManager::canUndo() const
{
    return !undoStack.empty();
}

bool CommandManager::canRedo() const
{
    return !redoStack.empty();
}

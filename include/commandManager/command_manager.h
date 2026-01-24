#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

// #include <memory>
#include <QVector>
#include "command.h"

class CommandManager
{
public:
    CommandManager() = default;
    
    void execute(std::shared_ptr<Command> command);
    void undo();
    void redo();
    
    bool can_undo() const;
    bool can_redo() const;
    
private:
    QVector<std::shared_ptr<Command>> m_undo_stack;
    QVector<std::shared_ptr<Command>> m_redo_stack;
};

#endif // COMMAND_MANAGER_H

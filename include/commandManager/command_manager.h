#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include <memory>
#include <QGraphicsScene>
#include <QVector>
#include "command.h"

class CommandManager
{
public:
    CommandManager(QGraphicsScene* scene);
    
    void execute(std::shared_ptr<Command> command);
    void undo();
    void redo();
    
    bool canUndo() const;
    bool canRedo() const;
    
private:
    QGraphicsScene* scene;
    QVector<std::shared_ptr<Command>> undoStack;
    QVector<std::shared_ptr<Command>> redoStack;
};

#endif // COMMAND_MANAGER_H

#ifndef MOVE_NODE_COMMAND_H
#define MOVE_NODE_COMMAND_H

#include "command.h"
#include <QPointF>
#include <QPropertyAnimation>

class CppClassView;

class MoveNode : public Command
{
public:
    MoveNode(CppClassView* item, QPointF delta);

    bool execute() override;
    bool undo() override;

private:
    void animate(QPointF from, QPointF to);

    CppClassView* mItem;
    QPointF mDelta;
};

#endif // MOVE_NODE_COMMAND_H

#ifndef MOVE_NODE_COMMAND_H
#define MOVE_NODE_COMMAND_H

#include "command.h"
#include <QObject>
#include <QPointF>
#include <QThread>

class CppClassView;

class MoveNode : public Command
{
public:
    MoveNode(CppClassView* item, QPointF delta);

    bool execute() override;
    bool undo() override;

private:
    CppClassView* mItem;
    QPointF mDelta;
};

class AnimateMove : public QThread {
    Q_OBJECT
public:
    AnimateMove(CppClassView* item, QPointF delta, QObject* parent = nullptr);

protected:
    void run() override;

signals:
    void moveStep(QPointF miniDelta);

private:
    CppClassView* mItem;
    QPointF mDelta;
};

#endif // MOVE_NODE_COMMAND_H

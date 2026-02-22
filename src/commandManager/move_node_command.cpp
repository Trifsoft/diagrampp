#include "commandManager/move_node_command.h"
#include "view/cpp_class_view.h"
#include <cmath>

// MoveNode

MoveNode::MoveNode(CppClassView* item, QPointF delta)
    : mItem(item), mDelta(delta)
{}

bool MoveNode::execute()
{
    auto* anim = new AnimateMove(mItem, mDelta);
    QObject::connect(anim, &QThread::finished, anim, &QObject::deleteLater);
    anim->start();
    return true;
}

bool MoveNode::undo()
{
    auto* anim = new AnimateMove(mItem, -mDelta);
    QObject::connect(anim, &QThread::finished, anim, &QObject::deleteLater);
    anim->start();
    return true;
}

// AnimateMove

AnimateMove::AnimateMove(CppClassView* item, QPointF delta, QObject* parent)
    : QThread(parent)
    , mItem(item)
    , mDelta(delta)
{
    connect(this, &AnimateMove::moveStep, mItem, [item](QPointF miniDelta) {
        item->setPos(item->pos() + miniDelta);
    }, Qt::QueuedConnection);
}

void AnimateMove::run()
{
    const double refreshRate = 60.0;
    const int intervalMs = static_cast<int>(std::ceil(1000.0 / refreshRate));
    const int steps = static_cast<int>(std::ceil(200 * refreshRate / 1000));
    const QPointF miniDelta = mDelta / steps;

    for (int i = 0; i < steps; i++) {
        emit moveStep(miniDelta);
        QThread::msleep(intervalMs);
    }
}

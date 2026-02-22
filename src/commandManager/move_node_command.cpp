#include "commandManager/move_node_command.h"
#include "view/cpp_class_view.h"
#include <QObject>

MoveNode::MoveNode(CppClassView* item, QPointF delta)
    : mItem(item), mDelta(delta)
{}

bool MoveNode::execute()
{
    animate(mItem->pos(), mItem->pos() + mDelta);
    return true;
}

bool MoveNode::undo()
{
    animate(mItem->pos(), mItem->pos() - mDelta);
    return true;
}

void MoveNode::animate(QPointF from, QPointF to)
{
    auto* anim = new QPropertyAnimation(mItem, "pos");
    anim->setDuration(300);
    anim->setStartValue(from);
    anim->setEndValue(to);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    QObject::connect(anim, &QPropertyAnimation::finished, anim, &QObject::deleteLater);
    anim->start();
}

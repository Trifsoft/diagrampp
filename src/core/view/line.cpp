#include "view/line.h"
#include <QGraphicsSceneMouseEvent>

Line::Line(BranchType branchType, const QLineF& line, QGraphicsItem* parent)
    : QGraphicsLineItem(line, parent)
    , m_branch_type(branchType)
    , mPen(QPen(Qt::gray, 4))
{
    if (branchType == BranchType::DEPENDENCY || branchType == BranchType::REALIZATION) {
        mPen.setStyle(Qt::DashLine);
    }
    setPen(mPen);
}

BranchType Line::branchType() const
{
    return m_branch_type;
}

void Line::select(bool isSelected)
{
    if(isSelected) {
        mPen.setBrush(Qt::red);
    }
    else {
        mPen.setBrush(Qt::gray);
    }
    setPen(mPen);
}

void Line::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsLineItem::mousePressEvent(event);
    event->accept();
}

void Line::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (boundingRect().contains(event->pos())) {
        emit objectClicked();
    }
    QGraphicsLineItem::mouseReleaseEvent(event);
}

void Line::move_start(const QPointF& delta) {
    QLineF l = line();
    l.setP1(l.p1() + delta);
    setLine(l);
}

void Line::move_end(const QPointF& delta) {
    QLineF l = line();
    l.setP2(l.p2() + delta);
    setLine(l);
}

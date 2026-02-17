#include "view/line.h"
#include <QPen>

Line::Line(BranchType branchType, const QLineF& line, QGraphicsItem* parent)
    : QGraphicsLineItem(line, parent)
    , m_branch_type(branchType)
{
    QPen pen(Qt::gray, 4);
    if (branchType == BranchType::DEPENDENCY || branchType == BranchType::REALIZATION) {
        pen.setStyle(Qt::DashLine);
    }
    setPen(pen);
}

BranchType Line::branchType() const
{
    return m_branch_type;
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

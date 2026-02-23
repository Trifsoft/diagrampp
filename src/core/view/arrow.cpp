#include "view/arrow.h"
#include<QGraphicsSceneMouseEvent>

Arrow::Arrow(BranchType branchType, const QPointF& parentPos, double height, qsizetype offset, QGraphicsItem* parent)
    : QGraphicsPolygonItem(parent)
    , m_branch_type(branchType)
    , mPen(QPen(Qt::black, 2))
{
    QPolygonF polygon;

    switch (branchType) {
    case BranchType::INHERITANCE:
        polygon << QPointF(-m_arrow_size / 2, m_arrow_size)
                << QPointF(m_arrow_size / 2, m_arrow_size)
                << QPointF(0, 0);
        setBrush(Qt::white);
        break;
    case BranchType::COMPOSITION:
        polygon << QPointF(0, 0)
                << QPointF(-m_arrow_size / 2, m_arrow_size / 2)
                << QPointF(0, m_arrow_size)
                << QPointF(m_arrow_size / 2, m_arrow_size / 2);
        setBrush(Qt::black);
        break;
    case BranchType::AGGREGATION:
        polygon << QPointF(0, 0)
                << QPointF(-m_arrow_size / 2, m_arrow_size / 2)
                << QPointF(0, m_arrow_size)
                << QPointF(m_arrow_size / 2, m_arrow_size / 2);
        setBrush(Qt::white);
        break;
    case BranchType::DEPENDENCY:
        polygon << QPointF(0, 0)
                << QPointF(-m_arrow_size / 3, m_arrow_size)
                << QPointF(m_arrow_size / 3, m_arrow_size);
        setBrush(Qt::blue);
        break;
    case BranchType::REALIZATION:
        polygon << QPointF(-m_arrow_size / 2, m_arrow_size)
                << QPointF(m_arrow_size / 2, m_arrow_size)
                << QPointF(0, 0);
        setBrush(Qt::white);
        break;
    case BranchType::ASSOCIATION:
        break;
    }

    setPolygon(polygon);
    setPen(mPen);

    setPos(parentPos + QPointF(offset * m_arrow_size, height));
}

QPointF Arrow::get_bottom_center() const {
    QRectF br = boundingRect();
    QPointF bottomCenter(br.center().x(), br.bottom());
    return mapToScene(bottomCenter);
}

void Arrow::select(bool isSelected)
{
    if(isSelected) {
        mPen.setBrush(Qt::red);
    }
    else {
        mPen.setBrush(Qt::transparent);
    }
    setPen(mPen);
}

void Arrow::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPolygonItem::mousePressEvent(event);
    event->accept();
}

void Arrow::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (boundingRect().contains(event->pos())) {
        emit objectClicked();
    }
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

void Arrow::move_by(const QPointF& delta) {
    setPos(pos() + delta);
    emit moved_by(delta);
}

void Arrow::move_back() {
    move_by(QPointF(-m_arrow_size, 0));
}

void Arrow::move_forward() {
    move_by(QPointF(m_arrow_size, 0));
}

void Arrow::move_y(double delta_y) {
    move_by(QPointF(0, delta_y));
}

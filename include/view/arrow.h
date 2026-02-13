#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsPolygonItem>
#include <QObject>
#include "model/base/branches.h"

class Arrow : public QObject, public QGraphicsPolygonItem {
    Q_OBJECT
public:
    explicit Arrow(BranchType branchType, const QPointF& parentPos, double height, qsizetype offset, QGraphicsItem* parent = nullptr);

    QPointF get_bottom_center() const;
signals:
    void moved_by(const QPointF&);
public slots:
    void move_by(const QPointF&);
    void move_y(double);
    void move_back();
private:
    qreal m_arrow_size = 15;
    BranchType m_branch_type;
};

#endif // ARROW_H

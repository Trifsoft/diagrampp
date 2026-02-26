#ifndef LINE_H
#define LINE_H

#include <QGraphicsLineItem>
#include <QObject>
#include <QPen>
#include "model/base/branches.h"

class Line : public QObject, public QGraphicsLineItem {
    Q_OBJECT
public:
    Line(BranchType branchType, const QLineF& line, QGraphicsItem* parent = nullptr);

    BranchType branchType() const;

    void select(bool);
public slots:
    void move_start(const QPointF& delta);
    void move_end(const QPointF& delta);

private:
    BranchType m_branch_type;
    QPen mPen;
};

#endif // LINE_H

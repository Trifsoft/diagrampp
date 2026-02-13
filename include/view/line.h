#ifndef LINE_H
#define LINE_H

#include <QGraphicsLineItem>
#include <QObject>
#include "model/base/branches.h"

class Line : public QObject, public QGraphicsLineItem {
    Q_OBJECT
public:
    explicit Line(BranchType branchType, const QLineF& line, QGraphicsItem* parent = nullptr);

public slots:
    void move_start(const QPointF& delta);
    void move_end(const QPointF& delta);

private:
    BranchType m_branch_type;
};

#endif // LINE_H

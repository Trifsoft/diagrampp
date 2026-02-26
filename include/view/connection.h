#ifndef CONNECTION_H
#define CONNECTION_H

#include <view/arrow.h>
#include <view/line.h>
#include <node_view_ptr.h>

class Connection: public QObject, public QGraphicsItemGroup {
    Q_OBJECT
public:
    Arrow* arrow;
    Line* line;
    Connection(NodeViewPtr, NodeViewPtr, int, BranchType);
    ~Connection();

    NodePtr from() const;
    NodePtr to() const;
    BranchType branchType() const;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

signals:
    void shown();
    void hidden();
private:
    NodePtr mFrom;
    NodePtr mTo;
    BranchType mBranchType;
};
#endif

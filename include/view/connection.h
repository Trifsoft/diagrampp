#ifndef CONNECTION_H
#define CONNECTION_H

#include <view/arrow.h>
#include <view/line.h>
#include <view/selectable.h>
#include <node_view_ptr.h>

class Connection: public QObject, public ISelectable {
    Q_OBJECT
public:
    Arrow* arrow;
    Line* line;
    Connection(NodeViewPtr, NodeViewPtr, int, BranchType);
    ~Connection();

    NodePtr from() const;
    NodePtr to() const;
    BranchType branchType() const;

    void select(bool) override;
signals:
    void shown();
    void hidden();
private:
    NodePtr mFrom;
    NodePtr mTo;
    BranchType mBranchType;
};
#endif

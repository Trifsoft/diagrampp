#include <view/connection.h>

Connection::Connection(NodeViewPtr child, NodeViewPtr parent, int arrowOffset, BranchType branch)
    : arrow(new Arrow(branch, parent->pos(), parent->boundingRect().height(), arrowOffset))
    , line(new Line(branch, QLineF(child->get_top_center(), arrow->get_bottom_center())))
    , mFrom(child->get_uml_class_diagram_node().get())
    , mTo(parent->get_uml_class_diagram_node().get())
    , mBranchType(branch)
{
    arrow->setParent(this);
    line->setParent(this);

    connect(arrow, &Arrow::moved_by, line, &Line::move_end);
    connect(child, &CppClassView::moved_by, line, &Line::move_start);

    connect(parent, &CppClassView::moved_by, arrow, &Arrow::move_by);
    connect(parent, &CppClassView::height_changed_by, arrow, &Arrow::move_y);

    connect(child, &QObject::destroyed, this, &QObject::deleteLater);
    connect(parent, &QObject::destroyed, this, &QObject::deleteLater);
}

BranchType Connection::branchType() const {
    return mBranchType;
}

void Connection::select(bool isSelected)
{
    arrow->select(isSelected);
    line->select(isSelected);
}

Connection::~Connection() {}

NodePtr Connection::from() const
{
    return mFrom;
}

NodePtr Connection::to() const
{
    return mTo;
}

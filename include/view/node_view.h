#ifndef NODE_VIEW_H
#define NODE_VIEW_H

#include <QGraphicsItem>
#include <model/base/uml_class_diagram_node.h>

class NodeView: public QGraphicsObject {
public:
    NodeView(QGraphicsItem* parent = nullptr);

    virtual IUMLClassDiagramNode* get_uml_class_diagram_node() = 0;
};

#endif // NODE_VIEW_H

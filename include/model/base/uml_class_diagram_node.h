#ifndef UML_CLASS_DIAGRAM_NODE_H
#define UML_CLASS_DIAGRAM_NODE_H

#include <QString>

class IUMLClassDiagramNode {
public:
    virtual ~IUMLClassDiagramNode() = default;

    virtual QString get_label() const = 0;
    virtual QString declaration() const = 0;
    virtual QString definition() const = 0;

    bool Clicked = false;
};

#endif // UML_CLASS_DIAGRAM_NODE_H

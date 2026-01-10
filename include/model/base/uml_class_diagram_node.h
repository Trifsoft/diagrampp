#ifndef UML_CLASS_DIAGRAM_NODE_H
#define UML_CLASS_DIAGRAM_NODE_H

#include <QString>

class IUMLClassDiagramNode {
public:
    virtual ~IUMLClassDiagramNode() = default;

    virtual QString declaration() const = 0;
    virtual QString definition() const = 0;
};

#endif // UML_CLASS_DIAGRAM_NODE_H

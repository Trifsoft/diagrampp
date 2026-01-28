#ifndef UML_CLASS_DIAGRAM_NODE_H
#define UML_CLASS_DIAGRAM_NODE_H

#include <QString>

class CppClass;

class IUMLClassDiagramNode : public std::enable_shared_from_this<IUMLClassDiagramNode> {
public:
    enum NodeLabel{
        ENUM,
        CLASS,
        STRUCT
    };
    virtual ~IUMLClassDiagramNode() = default;

    virtual QString get_name() const = 0;
    virtual NodeLabel get_label() const = 0;
    virtual QString declaration() const = 0;
    virtual QString definition() const = 0;

    bool Clicked = false;

};

#endif // UML_CLASS_DIAGRAM_NODE_H

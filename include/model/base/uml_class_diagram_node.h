#ifndef UML_CLASS_DIAGRAM_NODE_H
#define UML_CLASS_DIAGRAM_NODE_H

#include <QString>


class IUMLClassDiagramNode : public std::enable_shared_from_this<IUMLClassDiagramNode> {
public:
    virtual ~IUMLClassDiagramNode() = default;

    virtual QString get_name() const = 0;
    virtual QString get_label() const = 0;
    virtual QString declaration() const = 0;
    virtual QString definition() const = 0;

    bool Clicked = false;
    //CppClass *parent = nullptr;

};

#endif // UML_CLASS_DIAGRAM_NODE_H

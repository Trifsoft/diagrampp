#ifndef UML_CLASS_DIAGRAM_NODE_H
#define UML_CLASS_DIAGRAM_NODE_H

#include <QObject>
#include <QString>

class CppClass;

class IUMLClassDiagramNode: public QObject {
public:
    virtual ~IUMLClassDiagramNode() = default;

    virtual QString get_name() const = 0;
    virtual QString declaration() const = 0;
    virtual QString definition() const = 0;

    bool Clicked = false;

};

#endif // UML_CLASS_DIAGRAM_NODE_H

#ifndef DIAGRAM_GRAPH_H
#define DIAGRAM_GRAPH_H

#include <QList>
#include <QMap>
#include <QPair>
#include <memory>
#include <model/base/branches.h>

class IUMLClassDiagramNode;
class Composition;

class DiagramGraph {
public:

    using SharedNodePtr = std::shared_ptr<IUMLClassDiagramNode>;
    using WeakNodePtr = std::weak_ptr<IUMLClassDiagramNode>;

    DiagramGraph() = default;

    void addNode(SharedNodePtr node);
    void removeNode(SharedNodePtr node);
    void addLink(SharedNodePtr from, WeakNodePtr to, BranchType = BranchType::INHERITANCE);


    //obican pokazivac zato sto links ne poseduje node-ove
    QMap<IUMLClassDiagramNode*, QList<QPair<WeakNodePtr, BranchType>>> links;
    QList<SharedNodePtr> nodes;
private:
};

#endif // DIAGRAM_GRAPH_H

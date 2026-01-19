#ifndef DIAGRAM_GRAPH_H
#define DIAGRAM_GRAPH_H

#include <QList>
#include <QMap>
#include <QPair>
#include <memory>
#include <model/base/branches.h>
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

    SharedNodePtr firstNode = nullptr;
    SharedNodePtr secondNode = nullptr;
    void addLink(SharedNodePtr from, SharedNodePtr to, BranchType = BranchType::INHERITANCE);
    QMap<IUMLClassDiagramNode*, QList<QPair<WeakNodePtr, BranchType>>> links;
private:
    QList<SharedNodePtr> nodes;
};

#endif // DIAGRAM_GRAPH_H

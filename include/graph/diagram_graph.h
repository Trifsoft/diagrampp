#ifndef DIAGRAM_GRAPH_H
#define DIAGRAM_GRAPH_H

#include <vector>
#include <utility>
#include <QMap>
#include <model/base/branches.h>
#include <QObject>
#include <node_type.h>
#include <shared_node_ptr.h>
#include <graph/graph.h>

#define DEBUG_MODE 2

class IUMLClassDiagramNode;
class Composition;

using ConnectionList = QList<std::pair<SharedNodePtr, BranchType>>;
using graph_type = QMap<SharedNodePtr, ConnectionList>;

using BranchEdge = std::tuple<SharedNodePtr, SharedNodePtr, BranchType>;

class DiagramGraph : public QObject, public Graph<SharedNodePtr, BranchType> {
    Q_OBJECT
public:

    DiagramGraph() = default;
    ~DiagramGraph();

    std::vector<BranchEdge> get_branches_for_node(SharedNodePtr node) const;


    SharedNodePtr find_pointer_owner(NodePtr node_view);

#if DEBUG_MODE >= 1
    void show_diagram();
#endif

    graph_type& get_diagram();

public slots:
    void processNewNodeRequest(const QString&, NodeType);
    void processRemoveNodeRequest(NodePtr);
    void processNewBranchRequest(NodePtr, NodePtr, BranchType);
    void processRemoveBranchRequest(NodePtr, NodePtr, BranchType);

    void addBranch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
    void removeBranch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
    void addNode(SharedNodePtr);
    void removeNode(SharedNodePtr);

signals:
    void link_added(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void addBranchRequestApproved(SharedNodePtr from, SharedNodePtr to, BranchType branch);

    void link_removed(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void removeBranchRequestApproved(SharedNodePtr from, SharedNodePtr to, BranchType branch);

    void node_removed(SharedNodePtr node);
    void removeNodeRequestApproved(SharedNodePtr, const QList<BranchEdge>&);

    void node_added(SharedNodePtr node);
    void addNodeRequestApproved(SharedNodePtr);

    void error(const QString&);
protected:
    void onAddBranch(SharedNodePtr, SharedNodePtr, BranchType) override;
    void onRemoveBranch(SharedNodePtr, SharedNodePtr, BranchType) override;
    void onAddNode(SharedNodePtr) override;
    void onRemoveNode(SharedNodePtr) override;

private:

    bool connection_exists(SharedNodePtr from, SharedNodePtr to, BranchType branch_type) const;
    bool canCreateNode(const QString&) const;

};

#endif // DIAGRAM_GRAPH_H

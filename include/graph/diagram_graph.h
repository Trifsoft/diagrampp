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
    QList<BranchType> getAvailableBranchTypes(SharedNodePtr from, SharedNodePtr to) const;


    SharedNodePtr find_pointer_owner(NodePtr node_view);

#if DEBUG_MODE >= 1
    void show_diagram();
#endif

    graph_type& get_diagram();

public slots:
    void processNewNodeRequest(const QString&, NodeType);
    //void processRemoveNodeRequest(NodePtr);
    void processNewBranchRequest(NodePtr, NodePtr, BranchType);
    bool processRemoveBranchRequest(NodePtr, NodePtr, BranchType);
    void procesRemoveRequest(const QList<NodePtr>&,
                             const QList<std::tuple<NodePtr, NodePtr, BranchType>>&);

    void addBranch(NodePtr from, NodePtr to, BranchType branch_type);
    void removeBranch(NodePtr from, NodePtr to, BranchType branch_type);
    void addNode(NodePtr);
    void removeNode(NodePtr);

signals:
    void link_added(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void addBranchRequestApproved(NodePtr from, NodePtr to, BranchType branch);

    void link_removed(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    //void removeBranchRequestApproved(SharedNodePtr from, SharedNodePtr to, BranchType branch);

    void node_removed(SharedNodePtr node);
    //void removeNodeRequestApproved(SharedNodePtr, const QList<BranchEdge>&);

    void removeRequestApproved(const QList<NodePtr>&,
                               const QList<std::tuple<NodePtr, NodePtr, BranchType>>&);

    void node_added(SharedNodePtr node);
    void addNodeRequestApproved(NodePtr);

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

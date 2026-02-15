#ifndef DIAGRAM_GRAPH_H
#define DIAGRAM_GRAPH_H

#include <vector>
#include <utility>
#include <map>
#include <model/base/branches.h>
#include <QObject>
#include <node_type.h>
#include <shared_node_ptr.h>

#define DEBUG_MODE 2

class IUMLClassDiagramNode;
class Composition;

using graph_type = std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>;

class DiagramGraph : public QObject {
    Q_OBJECT
public:
    struct BranchEdge {
        SharedNodePtr from;
        SharedNodePtr to;
        BranchType branch_type;
    };

    DiagramGraph() = default;
    ~DiagramGraph() = default;

    void add_node(SharedNodePtr);
    void remove_node(SharedNodePtr);

    std::vector<BranchEdge> get_branches_for_node(SharedNodePtr node) const;

    bool add_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type, std::string& error_message);
    bool remove_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);

    SharedNodePtr find_pointer_owner(Composition *node_view);

#if DEBUG_MODE >= 1
    void show_diagram();
#endif

    graph_type& get_diagram();

public slots:
    void processNewNodeRequest(const QString&, NodeType);
    void processRemoveNodeRequest(const QString&, NodeType);

    void addNode(const QString&, NodeType);
    void removeNode(const QString&, NodeType);
signals:
    void link_added(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void link_removed(SharedNodePtr from, SharedNodePtr to, BranchType branch);

    void node_removed(SharedNodePtr node);
    void removeNodeRequestApproved(const QString&, NodeType);

    void node_added(SharedNodePtr node);
    void addNodeRequestApproved(const QString&, NodeType);

private:
    // disscussion, shared or weak
    graph_type m_diagram;
    void remove_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
    void add_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);

    bool connection_exists(SharedNodePtr from, SharedNodePtr to, BranchType branch_type) const;
    bool canCreateNode(const QString&) const;

};

#endif // DIAGRAM_GRAPH_H

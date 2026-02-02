#ifndef DIAGRAM_GRAPH_H
#define DIAGRAM_GRAPH_H

#include <vector>
#include <utility>
#include <map>
#include <model/base/branches.h>
#include <view/node_view.h>
#include <QObject>


#define DEBUG_MODE 2

class IUMLClassDiagramNode;
class Composition;

using SharedNodePtr = std::shared_ptr<NodeView>;
using graph_type = std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>;

class DiagramGraph : public QObject {
    Q_OBJECT
public:
    DiagramGraph() = default;
    ~DiagramGraph() = default;

    void add_node(SharedNodePtr node);
    void remove_node(SharedNodePtr node);

    void add_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
    void remove_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
    std::shared_ptr<NodeView> find_pointer_owner(NodeView *node_view);


    bool connection_exists(SharedNodePtr from, SharedNodePtr to, BranchType branch_type) const;

#if DEBUG_MODE >= 1
    void show_diagram();
#endif

    std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& get_diagram();

signals:
    void link_added(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void link_removed(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void node_removed(SharedNodePtr node);

private:
    // disscussion, shared or weak
    graph_type m_diagram;

    void remove_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
    void add_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
};

#endif // DIAGRAM_GRAPH_H

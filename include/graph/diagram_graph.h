#ifndef DIAGRAM_GRAPH_H
#define DIAGRAM_GRAPH_H

#include <vector>
#include <utility>
#include <map>
#include <model/base/branches.h>
#include <QObject>

#define DEBUG_MODE 2

class IUMLClassDiagramNode;
class Composition;

using SharedNodePtr = std::shared_ptr<Composition>;
using graph_type = std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>;

class DiagramGraph : public QObject {
    Q_OBJECT
public:
    DiagramGraph() = default;
    ~DiagramGraph() = default;

    void add_node(SharedNodePtr node);
    void remove_node(SharedNodePtr node);

    bool add_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type, std::string& error_message);
    bool remove_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);

    SharedNodePtr find_pointer_owner(Composition *node_view);

#if DEBUG_MODE >= 1
    void show_diagram();
#endif

    graph_type& get_diagram();

signals:
    void link_added(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void link_removed(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void node_removed(SharedNodePtr node);

private:
    // disscussion, shared or weak
    graph_type m_diagram;
    void remove_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
    void add_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
    bool connection_exists(SharedNodePtr from, SharedNodePtr to, BranchType branch_type) const;
};

#endif // DIAGRAM_GRAPH_H

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

using SharedNodePtr = NodeView*;

class DiagramGraph : public QObject {
    Q_OBJECT
public:
    DiagramGraph() = default;
    ~DiagramGraph() = default;

    // should be removed
    SharedNodePtr first_selected_node = nullptr;
    SharedNodePtr second_selected_node = nullptr;

    void add_node(SharedNodePtr node);
    void remove_node(SharedNodePtr node);

    void add_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
    void remove_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);

#if DEBUG_MODE >= 1
    void showDiagram();
#endif

    std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& get_diagram();

signals:
    void link_added(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void link_removed(SharedNodePtr from, SharedNodePtr to);

private:
    // disscussion, shared or weak
    std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>> diagram;

    void remove_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
    void add_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
};

#endif // DIAGRAM_GRAPH_H

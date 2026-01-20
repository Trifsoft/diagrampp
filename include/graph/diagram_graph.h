#ifndef DIAGRAM_GRAPH_H
#define DIAGRAM_GRAPH_H

#include <vector>
#include <utility>
#include <map>
#include <memory>
#include <model/base/branches.h>
#include <model/base/branches.h>

class IUMLClassDiagramNode;
class Composition;

using SharedNodePtr = std::shared_ptr<IUMLClassDiagramNode>;
using WeakNodePtr = std::weak_ptr<IUMLClassDiagramNode>;

class DiagramGraph {
public:
    static DiagramGraph& diagram_graph(){
        static DiagramGraph dg {};
        return dg;
    }

    // should be removed
    SharedNodePtr first_selected_node = nullptr;
    SharedNodePtr second_selected_node = nullptr;

    void add_node(SharedNodePtr node);
    void remove_node(SharedNodePtr node);

    bool add_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type, std::string& linkage_error_message);
    void remove_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);

    const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& get_diagram() const;
private:
    // disscussion, shared or weak
    std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>> diagram;

    void remove_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
    void add_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);

    DiagramGraph() : diagram() {};
    DiagramGraph(const DiagramGraph&) = delete;
    DiagramGraph& operator=(const DiagramGraph&) = delete;
};

#endif // DIAGRAM_GRAPH_H

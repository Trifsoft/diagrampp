#include "graph/diagram_graph.h"

void DiagramGraph::add_node(SharedNodePtr node) {
    if(diagram.find(node) != diagram.end()){
        return;
    }

    diagram[node] = {};
}

void DiagramGraph::remove_node(SharedNodePtr node) {
    if(diagram.find(node) == diagram.end()){
        return;
    }

    // erase node as a key
    diagram.erase(node);

    // erase all branches linked to node
    for(auto& it : diagram){
        auto& neighbours = it.second;
        for (auto neighbours_it = neighbours.begin(); neighbours_it != neighbours.end(); neighbours_it++){
            auto& [neighbour, _] = (*neighbours_it);
            if(neighbour == node){
                neighbours_it = neighbours.erase(neighbours_it);
            }
        }
    }
}

// prototype, until validator is implemented
bool DiagramGraph::add_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type, std::string linkage_error_message) {
    // if(!LinkageValidator::valid_linkage(from, to, type, linkage_error_message)){
    //     return false;
    // }

    switch(branch_type){
        case BranchType::ASSOCIATION:// ASSOCIATION -> undirected link
            add_neighbour(from, to, branch_type);
            add_neighbour(to, from, branch_type);
            break;
        default: // INHERITANCE & NAVIGATION -> directed links
            add_neighbour(from, to, branch_type);
            break;
    }

    return true;
}

void DiagramGraph::remove_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type){
    for(auto it = diagram[from].begin(); it != diagram[from].end(); it++){
        auto [neighbour, neighbour_branch_type] = (*it);
        if(neighbour == to && neighbour_branch_type == branch_type){
            diagram[from].erase(it);
            break;
        }
    }
}

void DiagramGraph::add_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type){
    diagram[from].push_back({to, branch_type});
}

void DiagramGraph::remove_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type) {
    switch(branch_type){
        case BranchType::ASSOCIATION:// ASSOCIATION -> undirected link
            remove_neighbour(from, to, branch_type);
            remove_neighbour(to, from, branch_type);
            break;
        default: // INHERITANCE & NAVIGATION -> directed links
            remove_neighbour(from, to, branch_type);
            break;
    }
}

const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& DiagramGraph::get_diagram() const{
    return diagram;
}

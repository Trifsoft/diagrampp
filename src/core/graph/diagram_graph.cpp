#include "graph/diagram_graph.h"

void DiagramGraph::add_node(SharedNodePtr node) {
    if(diagram.find(node) != diagram.end()){
        return;
    }

    diagram[node] = {};
}

void DiagramGraph::remove_node(SharedNodePtr node_to_remove) {
    if(diagram.find(node_to_remove) == diagram.end()){
        return;
    }

    diagram.erase(node_to_remove);

    // erase all branches linked to node
    for(auto& node_pair : diagram){
        auto& neighbours = node_pair.second;
        for(auto neighbours_it = neighbours.begin(); neighbours_it != neighbours.end(); ){
            auto& [neighbour_node, _] = (*neighbours_it);
            if(neighbour_node == node_to_remove){
                neighbours_it = neighbours.erase(neighbours_it);
            }else{
                neighbours_it++;
            }
        }
    }
    emit node_removed(node_to_remove);
}

void DiagramGraph::add_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type) {
    add_neighbour(from, to, branch_type);
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
    emit link_added(from, to, branch_type);
}

void DiagramGraph::remove_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type) {
    remove_neighbour(from, to, branch_type);
    emit link_removed(from, to, branch_type);
}

std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& DiagramGraph::get_diagram(){
    return diagram;
}

bool DiagramGraph::connection_exists(SharedNodePtr child, SharedNodePtr parent, BranchType branch_type) const {
    for(auto const& pair : diagram){
        if(pair.first == child){
            for(auto const& child_connections : pair.second){
                if(child_connections.first == parent && child_connections.second == branch_type){
                    return true;
                }
            }
        }
    }
    return false;
}

#if DEBUG_MODE >=1
void DiagramGraph::showDiagram(){
    for(auto &value : diagram){
        QDebug debug_stream = qDebug();
        debug_stream << value.first->get_uml_class_diagram_node()->get_name() << ":";
        std::vector<std::pair<SharedNodePtr, BranchType>>& sequence = value.second;
        for(auto &pairs : sequence){
            debug_stream << pairs.first->get_uml_class_diagram_node()->get_name();
        }
        qDebug() << "----";
    }
}
#endif

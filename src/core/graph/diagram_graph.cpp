#include "graph/diagram_graph.h"
#include <model/elements/composition/composition.h>

void DiagramGraph::add_node(SharedNodePtr node) {
    if(m_diagram.find(node) != m_diagram.end()){
        return;
    }

    m_diagram[node] = {};
}

void DiagramGraph::remove_node(SharedNodePtr node_to_remove) {
    if(m_diagram.find(node_to_remove) == m_diagram.end()){
        return;
    }

    // erase all branches from any node to node_to_remove
    for(auto& [_, neighbours] : m_diagram){
        for(auto neighbours_it = neighbours.begin(); neighbours_it != neighbours.end(); ){
            auto [neighbour_node, _] = (*neighbours_it);
            if(neighbour_node == node_to_remove){
                neighbours_it = neighbours.erase(neighbours_it);
            }else {
                neighbours_it++;
            }
        }
    }

    // erase all node_to_remove neighbours
    m_diagram.erase(node_to_remove);

    emit node_removed(node_to_remove);
}

void DiagramGraph::add_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type) {
    add_neighbour(from, to, branch_type);
    emit link_added(from, to, branch_type);

    if(branch_type == BranchType::ASSOCIATION){ // ASOCCIATION is undirected
        add_neighbour(to, from, branch_type);
    }

    return ;
}


SharedNodePtr DiagramGraph::find_pointer_owner(Composition* node_view)
{
    for(auto node : m_diagram){
        if(node.first.get() == node_view){
            return node.first;
        }

        for (auto [neighbour, _] : node.second){
            if(neighbour.get() == node_view){
                return neighbour;
            }
        }
    }

    return nullptr;
}


void DiagramGraph::remove_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type){
    for(auto it = m_diagram[from].begin(); it != m_diagram[from].end(); ){
        auto& [neighbour, neighbour_branch_type] = (*it);
        if(neighbour == to && neighbour_branch_type == branch_type){
            it = m_diagram[from].erase(it);
            break;
        }else {
            ++it;
        }
    }
}

void DiagramGraph::add_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type){
    m_diagram[from].push_back({to, branch_type});

    if(branch_type == BranchType::ASSOCIATION){
        m_diagram[from].push_back({to, branch_type});
    }
}

void DiagramGraph::remove_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type) {
    remove_neighbour(from, to, branch_type);
    emit link_removed(from, to, branch_type);

    if(branch_type == BranchType::ASSOCIATION){
        remove_neighbour(to, from, branch_type);
        emit link_removed(to, from, branch_type);
    }
}

graph_type& DiagramGraph::get_diagram(){
    return m_diagram;
}

bool DiagramGraph::connection_exists(SharedNodePtr start_node, SharedNodePtr end_node, BranchType branch_type) const {
    auto start_node_it = m_diagram.find(start_node);
    if(start_node_it == m_diagram.end()){
        return false;
    }

    auto start_node_neighbours = start_node_it->second;
    for(auto const& [start_node_neighbour, start_end_branch_type] : start_node_neighbours){
        if(start_node_neighbour == end_node){
            return true;
        }
    }
    return false;
}

#if DEBUG_MODE >=1
void DiagramGraph::show_diagram(){
    for(auto &value : m_diagram){
        QDebug debug_stream = qDebug();
        debug_stream << value.first->get_name() << ":";
        std::vector<std::pair<SharedNodePtr, BranchType>>& sequence = value.second;
        for(auto &pairs : sequence){
            debug_stream << pairs.first->get_name();
        }
        qDebug() << "----";
    }
}
#endif

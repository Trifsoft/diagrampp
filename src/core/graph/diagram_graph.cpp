#include "graph/diagram_graph.h"

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

    m_diagram.erase(node_to_remove);

    // erase all branches linked to node
    for(auto& node_pair : m_diagram){
        auto& neighbours = node_pair.second;
        auto neighbours_it = neighbours.begin();
        while(neighbours_it != neighbours.end()){
            auto& [neighbour_node, _] = (*neighbours_it);
            // [FILIP] BUG fix (erase will increment iterator)
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


std::shared_ptr<NodeView> DiagramGraph::find_pointer_owner(NodeView *node_view)
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
    for(auto it = m_diagram[from].begin(); it != m_diagram[from].end(); it++){
        auto [neighbour, neighbour_branch_type] = (*it);
        if(neighbour == to && neighbour_branch_type == branch_type){
            m_diagram[from].erase(it);
            break;
        }
    }
}

void DiagramGraph::add_neighbour(SharedNodePtr from, SharedNodePtr to, BranchType branch_type){

    m_diagram[from].push_back({to, branch_type});
    emit link_added(from, to, branch_type);

}

void DiagramGraph::remove_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type) {
    remove_neighbour(from, to, branch_type);
    emit link_removed(from, to, branch_type);
}

std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& DiagramGraph::get_diagram(){
    return m_diagram;
}

bool DiagramGraph::connection_exists(SharedNodePtr child, SharedNodePtr parent, BranchType branch_type) const {
    for(auto const& pair : m_diagram){
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
void DiagramGraph::showm_diagram(){
    for(auto &value : m_diagram){
        QDebug debug_stream = qDebug();
        debug_stream << value.first->get_uml_class_m_diagram_node()->get_name() << ":";
        std::vector<std::pair<SharedNodePtr, BranchType>>& sequence = value.second;
        for(auto &pairs : sequence){
            debug_stream << pairs.first->get_uml_class_m_diagram_node()->get_name();
        }
        qDebug() << "----";
    }
}
#endif

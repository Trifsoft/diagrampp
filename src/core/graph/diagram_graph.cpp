#include "graph/diagram_graph.h"

void DiagramGraph::add_node(SharedNodePtr node) {
    if(m_diagram.find(node) != m_diagram.end()){
        return;
    }

    m_diagram[node] = {};
}

void DiagramGraph::remove_node(SharedNodePtr node) {
    if(m_diagram.find(node) == m_diagram.end()){
        return;
    }

    // erase node as a key
    m_diagram.erase(node);

    // erase all branches linked to node
    for(auto& it : m_diagram){
        auto& neighbours = it.second;
        for (auto neighbours_it = neighbours.begin(); neighbours_it != neighbours.end(); neighbours_it++){
            auto& [neighbour, _] = (*neighbours_it);
            if(neighbour == node){
                neighbours_it = neighbours.erase(neighbours_it);
            }
        }
    }
}

void DiagramGraph::add_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type) {
    switch(branch_type){
        case BranchType::ASSOCIATION:// ASSOCIATION -> undirected link
            add_neighbour(from, to, branch_type);
            add_neighbour(to, from, branch_type);
            break;
        default: // INHERITANCE & NAVIGATION -> directed links
            add_neighbour(from, to, branch_type);
            break;
    }
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

std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& DiagramGraph::get_diagram(){
    return m_diagram;
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

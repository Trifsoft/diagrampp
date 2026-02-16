#include "graph/diagram_graph.h"
#include <model/elements/composition/composition.h>
#include <validator.h>
#include <QDebug>

#include <model/elements/composition/cpp_class.h>
#include <model/elements/composition/cpp_struct.h>

void DiagramGraph::addNode(SharedNodePtr node) {
    ConnectionList list;
    if(m_removed.contains(node)) {
        list = m_removed.take(node);
    }
    else {
        list = {};
    }
    m_diagram[node] = list;
    emit node_added(node);
}
void DiagramGraph::removeNode(SharedNodePtr node) {
    if(node == nullptr){
        return;
    }

    auto list = m_diagram.take(node);
    m_removed[node] = {};

    emit node_removed(node);
}

std::vector<DiagramGraph::BranchEdge> DiagramGraph::get_branches_for_node(SharedNodePtr node) const {
    std::vector<BranchEdge> branches;
    if(!node){
        return branches;
    }

    // find all branches where node is either from or to node
    for(auto it = m_diagram.begin(); it != m_diagram.end(); it++){
        auto from_node = it.key();
        auto neighbours = it.value();
        for(const auto& [to_node, branch_type] : neighbours){
            if(from_node == node || to_node == node){
                branches.push_back({from_node, to_node, branch_type});
            }
        }
    }

    return branches;
}


void DiagramGraph::processNewBranchRequest(NodePtr fromRaw, NodePtr toRaw, BranchType branch_type) {
    auto from = find_pointer_owner(fromRaw);
    auto to = find_pointer_owner(toRaw);
    if(!connection_exists(from, to, branch_type)){
        addBranch(from, to, branch_type);

        std::string error_message;
        if(!Validator::validate(error_message, from, to, branch_type, m_diagram)){
            removeBranch(from, to, branch_type);
            emit linkError(error_message);
            return;
        }
    } else {
        emit linkError("Connection already exists.");
        return;
    }

    emit addBranchRequestApproved(from, to, branch_type);
}


SharedNodePtr DiagramGraph::find_pointer_owner(Composition* node_view)
{
    for(auto it = m_diagram.begin(); it != m_diagram.end(); it++){
        if(it.key().get() == node_view){
            return it.key();
        }
        for (auto [neighbour, _] : it.value()){
            if(neighbour.get() == node_view){
                return neighbour;
            }
        }
    }
    return nullptr;
}


void DiagramGraph::removeBranch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type){
    if(m_diagram.find(from) == m_diagram.end()){
        return ;
    }

    bool branchFound = false;
    for(auto it = m_diagram[from].begin(); it != m_diagram[from].end(); it++){
        auto& [neighbour, neighbour_branch_type] = (*it);
        if(neighbour == to && neighbour_branch_type == branch_type){
            m_diagram[from].erase(it);
            branchFound = true;
            break;
        }
    }
    if(branchFound) {
        m_removed_branches.append({from, to, branch_type});
        emit link_removed(from, to, branch_type);
    }
    else {
        emit linkError("Branch not found");
    }
}

void DiagramGraph::addBranch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type){
    if(branch_type == BranchType::INHERITANCE){
        from->inherits(Visibility::Public, to.get());
    }
    m_removed_branches.removeAll({from, to, branch_type});
    m_diagram[from].push_back({to, branch_type});
    emit link_added(from, to, branch_type);
}

void DiagramGraph::processRemoveBranchRequest(NodePtr fromRaw, NodePtr toRaw, BranchType branch_type) {
    auto from = find_pointer_owner(fromRaw);
    auto to = find_pointer_owner(toRaw);
    if(!connection_exists(from, to, branch_type)){
        emit linkError("Link doesn't exist");
        return;
    }

    if(branch_type == BranchType::INHERITANCE){
        from->break_inheritance();
    }

    if(branch_type == BranchType::ASSOCIATION){
        removeBranch(to, from, branch_type);
    }

    removeBranch(from, to, branch_type);
    emit removeBranchRequestApproved(from, to, branch_type);
}

graph_type& DiagramGraph::get_diagram(){
    return m_diagram;
}

bool DiagramGraph::canCreateNode(const QString &name) const {
    return true;
}

void DiagramGraph::processNewNodeRequest(const QString &name, NodeType nodeType)
{
    if(canCreateNode(name)) {
        SharedNodePtr node;
        switch(nodeType) {
            case NodeType::Class: {
                node = std::make_shared<CPPClass>(name);
                break;
            }
            case NodeType::Struct: {
                node = std::make_shared<CPPStruct>(name);
                break;
            }
        }
        addNode(node);
        emit addNodeRequestApproved(node);
    }
}
void DiagramGraph::processRemoveNodeRequest(NodePtr node) {
    if(node != nullptr) {
        auto sharedNode = find_pointer_owner(node);
        emit removeNodeRequestApproved(sharedNode);
        removeNode(sharedNode);
    }
}

bool DiagramGraph::connection_exists(SharedNodePtr start_node, SharedNodePtr end_node, BranchType branch_type) const {
    auto start_node_it = m_diagram.find(start_node);
    if(start_node_it == m_diagram.end() && branch_type != BranchType::ASSOCIATION){
        return false;
    }

    auto start_node_neighbours = start_node_it.value();
    for(auto const& [start_node_neighbour, start_end_branch_type] : start_node_neighbours){
        if(start_node_neighbour == end_node){
            return true;
        }
    }

    if(branch_type == BranchType::ASSOCIATION){
        auto end_node_it = m_diagram.find(end_node);
        auto neighbours = end_node_it.value();
        for(auto const& [end_node_neighbour, end_start_branch_type] : neighbours){
            if(end_node_neighbour == start_node && end_start_branch_type == BranchType::ASSOCIATION){
                return true;
            }
        }
    }

    return false;
    }

#if DEBUG_MODE >=1
void DiagramGraph::show_diagram(){
    for(auto it = m_diagram.begin(); it != m_diagram.end(); it++){
        QDebug debug_stream = qDebug();
        debug_stream << it.key()->get_name() << ":";
        qDebug() << it.key()->get_name() << ":";
        ConnectionList& sequence = it.value();
        for(auto &pairs : sequence){
            debug_stream << pairs.first->get_name();
            qDebug() << pairs.first->get_name();
        }
        qDebug() << "----";
    }
}
#endif

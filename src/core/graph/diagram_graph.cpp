#include "graph/diagram_graph.h"
#include <model/elements/composition/composition.h>
#include <validator.h>
#include <QDebug>

#include <model/elements/composition/cpp_class.h>
#include <model/elements/composition/cpp_struct.h>

void DiagramGraph::addNode(NodePtr nodeRaw) {
    auto node = find_pointer_owner(nodeRaw);
    if(!node) {
        emit error("nullptr");
    }
    mAddNode(node);
}
void DiagramGraph::removeNode(NodePtr nodeRaw) {
    auto node = find_pointer_owner(nodeRaw);
    if(node == nullptr){
        return;
    }
    mRemoveNode(node);
}

void DiagramGraph::onAddBranch(SharedNodePtr from, SharedNodePtr to, BranchType branchType)
{
    qDebug() << "Added branch";
    emit link_added(from, to, branchType);
}

void DiagramGraph::onRemoveBranch(SharedNodePtr from, SharedNodePtr to, BranchType branchType) {
    qDebug() << "Removed branch";
    emit link_removed(from, to, branchType);
}

void DiagramGraph::onAddNode(SharedNodePtr node) {
    qDebug() << "Added node";
    emit node_added(node);
}

void DiagramGraph::onRemoveNode(SharedNodePtr node) {
    qDebug() << "Removed node";
    emit node_removed(node);
}

DiagramGraph::~DiagramGraph()
{
    qDebug() << "Deleted diagram";
}

std::vector<BranchEdge> DiagramGraph::get_branches_for_node(SharedNodePtr node) const {
    std::vector<BranchEdge> branches;
    if(!node){
        return branches;
    }

    // find all branches where node is either from or to node
    for(auto it = nodeMap.begin(); it != nodeMap.end(); it++){
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


QList<BranchType> DiagramGraph::getAvailableBranchTypes(SharedNodePtr from, SharedNodePtr to) const {
    static const QList<BranchType> all = {
        BranchType::INHERITANCE,
        BranchType::ASSOCIATION,
        BranchType::REALIZATION,
        BranchType::AGGREGATION,
        BranchType::COMPOSITION,
        BranchType::DEPENDENCY
    };
    QList<BranchType> available;
    for(auto type : all) {
        if(!connection_exists(from, to, type)) {
            available.append(type);
        }
    }
    return available;
}

void DiagramGraph::processNewBranchRequest(NodePtr fromRaw, NodePtr toRaw, BranchType branch_type) {
    auto from = find_pointer_owner(fromRaw);
    auto to = find_pointer_owner(toRaw);
    if(!connection_exists(from, to, branch_type)){
        addBranch(fromRaw, toRaw, branch_type);

        QString error_message;
        if(!Validator::validate(error_message, from, to, branch_type, nodeMap)){
            removeBranch(fromRaw, toRaw, branch_type);
            emit error(error_message);
            return;
        }
    } else {
        emit error("Connection already exists.");
        return;
    }

    emit addBranchRequestApproved(from.get(), to.get(), branch_type);
}


SharedNodePtr DiagramGraph::find_pointer_owner(NodePtr node_view)
{
    for(auto& node : nodes) {
        if(node.get() == node_view) {
            return node;
        }
    }
    for(auto& node : removedNodes) {
        if(node.get() == node_view) {
            return node;
        }
    }
    return nullptr;
}


void DiagramGraph::removeBranch(NodePtr fromRaw, NodePtr toRaw, BranchType branch_type){
    auto from = find_pointer_owner(fromRaw);
    auto to = find_pointer_owner(toRaw);
    if(!mRemoveBranch(from, to, branch_type)) {
        emit error("Branch not found: " + QString::number(static_cast<int>(branch_type)));
    }
}

void DiagramGraph::addBranch(NodePtr fromRaw, NodePtr toRaw, BranchType branch_type){
    if(branch_type == BranchType::INHERITANCE){
        fromRaw->inherits(Visibility::Public, toRaw);
    }
    auto from = find_pointer_owner(fromRaw);
    auto to = find_pointer_owner(toRaw);
    mAddBranch(from, to, branch_type);
}

bool DiagramGraph::processRemoveBranchRequest(NodePtr fromRaw,
                                              NodePtr toRaw,
                                              BranchType branch_type) {
    auto from = find_pointer_owner(fromRaw);
    auto to = find_pointer_owner(toRaw);
    if(!connection_exists(from, to, branch_type)){
        return false;
    }

    // if(branch_type == BranchType::INHERITANCE){
    //     from->break_inheritance();
    // }

    // removeBranch(from, to, branch_type);
    return true;
    //emit removeBranchRequestApproved(from, to, branch_type);
}

void DiagramGraph::procesRemoveRequest(const QList<NodePtr> &nodes,
                                       const QList<std::tuple<NodePtr, NodePtr, BranchType> >& branches)
{
    for(auto& [iFrom, iTo, iBranch] : branches) {
        if(!processRemoveBranchRequest(iFrom, iTo, iBranch)) {
            emit error("Link doesn't exist!");
            return;
        }
    }
    for(auto& [iFrom, iTo, iBranch]: branches) {
        auto from = find_pointer_owner(iFrom);
        auto to = find_pointer_owner(iTo);
        mRemoveBranch(from, to, iBranch);
    }
    for(auto& iNode : nodes) {
        auto node = find_pointer_owner(iNode);
        mRemoveNode(node);
    }
    emit removeRequestApproved(nodes, branches);
}

graph_type& DiagramGraph::get_diagram(){
    return nodeMap;
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
        mAddNode(node);
        emit addNodeRequestApproved(node.get());
    }
}
// void DiagramGraph::processRemoveNodeRequest(NodePtr node) {
//     if(node != nullptr) {
//         auto sharedNode = find_pointer_owner(node);
//         QList<BranchEdge> branches = {};
//         for(auto it = nodeMap.begin(); it != nodeMap.end(); it++) {
//             for(auto& [iTo, iConn] : it.value()) {
//                 if(it.key() == sharedNode || iTo == sharedNode) {
//                     branches.append({it.key(), iTo, iConn});
//                     mRemoveBranch(it.key(), iTo, iConn);
//                 }
//             }
//         }
//         mRemoveNode(sharedNode);
//         emit removeNodeRequestApproved(sharedNode, branches);
//     }
// }

bool DiagramGraph::connection_exists(SharedNodePtr start_node, SharedNodePtr end_node, BranchType branch_type) const {
    auto fromConnections = nodeMap.find(start_node);
    if(fromConnections == nodeMap.end()) {
        return false;
    }
    for(auto const& [start_node_neighbour, start_end_branch_type] : nodeMap[start_node]){
        if(start_node_neighbour == end_node && start_end_branch_type == branch_type){
            return true;
        }
    }

    return false;
}

#if DEBUG_MODE >=1
void DiagramGraph::show_diagram(){
    for(auto it = nodeMap.begin(); it != nodeMap.end(); it++){
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

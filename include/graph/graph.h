#ifndef GRAPH_H
#define GRAPH_H

#include <QMap>
#include <QList>

#include <QDebug>

template <typename Node, typename Connection>
class Graph {
public:
    const QList<Node>& getNodes() const { return nodes; }
protected:
    QList<Node> nodes;
    QMap<Node, QList<std::pair<Node, Connection>>> nodeMap;
    QList<Node> removedNodes;
    QList<std::tuple<Node, Node, Connection>> removedBranches;

    virtual void onAddBranch(Node, Node, Connection) = 0;
    virtual void onRemoveBranch(Node, Node, Connection) = 0;
    virtual void onAddNode(Node) = 0;
    virtual void onRemoveNode(Node) = 0;

    void mAddBranch(Node from, Node to, Connection connection) {
        removedBranches.removeAll({from, to, connection});
        nodeMap[from].push_back({to, connection});
        onAddBranch(from, to, connection);
    }
    bool mRemoveBranch(Node from, Node to, Connection connection) {
        if(nodeMap.find(from) == nodeMap.end()) {
            return false;
        }
        bool branchFound = false;
        for(auto it = nodeMap[from].begin(); it != nodeMap[from].end(); it++){
            if(it->first == to && it->second == connection){
                nodeMap[from].erase(it);
                branchFound = true;
                break;
            }
        }
        if(branchFound) {
            removedBranches.append({from, to, connection});
            onRemoveBranch(from, to, connection);
        }
        return branchFound;
    }
    void mAddNode(Node node) {
        if(removedNodes.contains(node)) {
            removedNodes.removeAll(node);
        }
        else {
            nodeMap[node] = {};
        }
        nodes.append(node);
        onAddNode(node);
    }
    void mRemoveNode(Node node) {
        nodeMap.remove(node);
        removedNodes.append(node);
        onRemoveNode(node);
    }
};

#endif

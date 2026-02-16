#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include <QObject>
#include <shared_node_ptr.h>
#include <model/base/branches.h>

enum class NodeModification {
    Link,
    Remove,
    None
};

class ConnectionHandler: public QObject {
    Q_OBJECT
public:
    ConnectionHandler(QObject* parent = nullptr): QObject(parent) {}

    NodeModification getSelectedNodeModification();
public slots:
    void updateSelectedNodeModification(NodeModification);
    void updateSelectedBranchType(BranchType);
    void handleNodeClick(NodePtr);
signals:
    void nodeRemovalRequested(NodePtr);
    void branchCreationRequested(NodePtr, NodePtr, BranchType);
    void branchRemovalRequested(NodePtr, NodePtr, BranchType);
private:
    NodePtr first;
    NodePtr second;
    BranchType selectedBranchType = BranchType::INHERITANCE;
    NodeModification selectedNodeModification = NodeModification::None;
};

#endif

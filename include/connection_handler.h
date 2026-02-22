#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include <QObject>
#include <shared_node_ptr.h>
#include <view/selectable.h>
#include <node_view_ptr.h>
#include <model/base/branches.h>

#include <node_modification.h>

class ConnectionHandler: public QObject {
    Q_OBJECT
public:
    ConnectionHandler(QObject* parent = nullptr): QObject(parent) {}
    ~ConnectionHandler();

    NodeModification getSelectedNodeModification();
public slots:
    void updateSelectedNodeModification(NodeModification);
    void updateSelectedBranchType(BranchType);
    void handleClick(ISelectable*);
    void deselectAll();
    void processRemovalRequest();
signals:
    void nodeRemovalRequested(NodePtr);
    void branchCreationRequested(NodePtr, NodePtr, BranchType);
    void branchRemovalRequested(NodePtr, NodePtr, BranchType);

    void deselectionRequested(ISelectable*);
    void selectionRequested(ISelectable*);
private:
    ISelectable* selected;
    BranchType selectedBranchType = BranchType::INHERITANCE;
    NodeModification selectedNodeModification = NodeModification::None;

    void setSelected(ISelectable*);
};

#endif

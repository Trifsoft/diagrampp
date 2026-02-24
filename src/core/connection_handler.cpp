#include <connection_handler.h>
#include <QDebug>
#include <view/connection.h>

ConnectionHandler::~ConnectionHandler()
{
    qDebug() << "Deleted connection handler";
}

void ConnectionHandler::handleNodeRightClick(NodeViewPtr target)
{
    auto selectedAsNodeViewPtr = dynamic_cast<NodeViewPtr>(selected);
    if(selectedAsNodeViewPtr && selectedAsNodeViewPtr != target) {
        emit branchMenuRequested(selectedAsNodeViewPtr, target);
    }
    else {
        emit changeNodeRequested(target);
    }
}

void ConnectionHandler::deselectAll()
{
    setSelected(nullptr);
}

void ConnectionHandler::processRemovalRequest()
{
    if(selected != nullptr) {
        auto selectedAsNode = dynamic_cast<NodeViewPtr>(selected);
        auto selectedAsConnection = dynamic_cast<Connection*>(selected);
        if(selectedAsNode != nullptr) {
            emit nodeRemovalRequested(selectedAsNode->get_uml_class_diagram_node().get());
        }
        else if(selectedAsConnection != nullptr){
            emit branchRemovalRequested(selectedAsConnection->from(), selectedAsConnection->to(), selectedAsConnection->branchType());
        }
        setSelected(nullptr);
    }
}

void ConnectionHandler::setSelected(ISelectable* newSelectedNode)
{
    if(selected != nullptr) {
        emit deselectionRequested(selected);
    }
    if(selected != newSelectedNode) {
        selected = newSelectedNode;
        if(newSelectedNode != nullptr) {
            emit selectionRequested(newSelectedNode);
        }
    }
    else {
        selected = nullptr;
    }
}

#include <connection_handler.h>
#include <QDebug>
#include <view/connection.h>

ConnectionHandler::~ConnectionHandler()
{
    qDebug() << "Deleted connection handler";
}

bool ConnectionHandler::getSelectedNodeModification()
{
    return selectedNodeModification;
}

void ConnectionHandler::updateSelectedNodeModification(bool newNodeModification) {
    selectedNodeModification = newNodeModification;
    setSelected(nullptr);
}

void ConnectionHandler::updateSelectedBranchType(BranchType newBranchType) {
    selectedBranchType = newBranchType;
}

void ConnectionHandler::handleClick(ISelectable* newSelected) {
    if(!selectedNodeModification){
        return;
    }
    auto selectedNodePtr = dynamic_cast<NodeViewPtr>(selected);
    auto newSelectedNodePtr = dynamic_cast<NodeViewPtr>(newSelected);
    if(selectedNodePtr == nullptr || newSelectedNodePtr == nullptr ) {
        setSelected(newSelected);
    }
    else {
        NodePtr first = selectedNodePtr->get_uml_class_diagram_node().get();
        NodePtr second = newSelectedNodePtr->get_uml_class_diagram_node().get();

        if((first == second) && selectedNodeModification){
            //execute_remove_node_with_branches(first);
            emit nodeRemovalRequested(first);
        }
        else {
            emit branchCreationRequested(first, second, selectedBranchType);
        }

        setSelected(nullptr);
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
    selected = newSelectedNode;
    if(newSelectedNode != nullptr) {
        emit selectionRequested(newSelectedNode);
    }
}

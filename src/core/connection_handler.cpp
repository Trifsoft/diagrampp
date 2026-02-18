#include <connection_handler.h>
#include <QDebug>

ConnectionHandler::~ConnectionHandler()
{
    qDebug() << "Deleted connection handler";
}

NodeModification ConnectionHandler::getSelectedNodeModification()
{
    return selectedNodeModification;
}

void ConnectionHandler::updateSelectedNodeModification(NodeModification newNodeModification) {
    selectedNodeModification = newNodeModification;
    first = second = nullptr;
}

void ConnectionHandler::updateSelectedBranchType(BranchType newBranchType) {
    selectedBranchType = newBranchType;
}

void ConnectionHandler::handleNodeClick(NodePtr node) {
    if(selectedNodeModification == NodeModification::None){
        return;
    }
    if(!first){
        first = node;
        return;
    }
    second = node;

    if((first == second) && selectedNodeModification == NodeModification::Remove){
        //execute_remove_node_with_branches(first);
        emit nodeRemovalRequested(first);
        first = second = nullptr;
        return;
    }

    if((first && second) && selectedNodeModification == NodeModification::Link){
        //execute_add_branch(first, second, selectedBranchType);
        emit branchCreationRequested(first, second, selectedBranchType);
    } else if((first && second) && selectedNodeModification == NodeModification::Remove){
        //execute_remove_branch(first, second, selectedBranchType);
        emit branchRemovalRequested(first, second, selectedBranchType);
    }

    #if DEBUG_MODE>=1
        qDebug() << "";
        diagram->show_diagram();
    #endif

    first = second = nullptr;
}

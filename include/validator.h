#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "model/base/branches.h"
#include "graph/diagram_graph.h"



namespace Validator {

    bool validateDiagram(std::string& errorMessage, NodeView* from, NodeView* to,
                        BranchType branchType,
                        std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    bool checkLinkage(std::string& errorMessage, NodeView* from, NodeView* to);
    bool validateInheritance(std::string& errorMessage, NodeView* from, NodeView* to, std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    bool validateOthers(std::string& errorMessage, BranchType branchTye, NodeView* from, NodeView* to, std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    bool diamond(BranchType branchType, std::set<NodeView*>& stack, NodeView* node ,std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    bool checkDuplication(std::string& errorMessage, NodeView* child, NodeView* parent, std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    bool dfs(NodeView* node, std::unordered_map<NodeView*,bool>& visited,
             std::unordered_map<NodeView*,bool>& in_stack, std::map<SharedNodePtr,
             std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    bool hasCycle(NodeView* start, std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
};

#endif // VALIDATOR_H

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "model/base/branches.h"
#include "graph/diagram_graph.h"



namespace Validator {


    bool validate(std::string& errorMessage, SharedNodePtr from, SharedNodePtr to,
                        BranchType branchType,
                        const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    bool checkLinkage(std::string& errorMessage, SharedNodePtr from, SharedNodePtr to);
    bool check_multiple_conneciton(SharedNodePtr child, SharedNodePtr parent,
                                   const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    bool validateInheritance(std::string& errorMessage, SharedNodePtr from, SharedNodePtr to,
                             const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    bool validateOthers(std::string& errorMessage, BranchType branchTye, SharedNodePtr from, SharedNodePtr to,
                        const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    bool dfs(SharedNodePtr node, std::unordered_map<SharedNodePtr,bool>& visited,
             std::unordered_map<SharedNodePtr,bool>& in_stack,
             const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    bool hasCycle(SharedNodePtr start, BranchType branch_type,
                  const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    bool diamond(const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    bool detect_diamond(SharedNodePtr start, const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
    void paths_to_base(SharedNodePtr node,
                       const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram,
                       std::unordered_map<SharedNodePtr, int>& reach_count);
};

#endif // VALIDATOR_H

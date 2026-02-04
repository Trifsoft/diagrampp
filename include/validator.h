#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "model/base/branches.h"
#include "graph/diagram_graph.h"



namespace Validator {
    bool validate(std::string& errorMessage, SharedNodePtr from, SharedNodePtr to,
                  BranchType branchType,
                  const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram);
};

#endif // VALIDATOR_H

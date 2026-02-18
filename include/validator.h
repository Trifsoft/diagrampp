#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "model/base/branches.h"
#include "graph/diagram_graph.h"

namespace Validator {
    bool validate(QString& errorMessage, SharedNodePtr from, SharedNodePtr to,
                  BranchType branchType,
                  const graph_type& diagram);
};

namespace testvalidator{
    bool check_multiple_connection(SharedNodePtr child, SharedNodePtr parent,
                               const graph_type& diagram);

    bool check_cycle(SharedNodePtr start,
                     BranchType branch_type,
                     const graph_type& diagram);

    bool detect_diamond(SharedNodePtr start, const graph_type& diagram);

    void paths_to_base(SharedNodePtr node,
                       const graph_type& diagram,
                       std::unordered_map<SharedNodePtr, int>& reach_count);
    bool dfs(SharedNodePtr node,
             BranchType branch_type,
             std::unordered_map<SharedNodePtr,bool>& in_stack,
             const graph_type& diagram);
    };

#endif // VALIDATOR_H

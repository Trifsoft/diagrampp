#include "validator.h"

#include <unordered_set>

namespace Validator{

    namespace{
        bool check_multiple_connection(SharedNodePtr child, SharedNodePtr parent,
                                       const graph_type& diagram){
            auto value = diagram[child];
            int count = 0;
            for(auto& pair : value){
                if(pair.first == parent){
                    count++;
                }
            }
            return count < 2;
        }

        bool dfs(SharedNodePtr node,
                 BranchType branch_type,
                 std::unordered_map<SharedNodePtr,bool>& in_stack,
                 const graph_type& diagram){
            in_stack[node] = true;

            auto it = diagram[node];
            for(auto& pair : it){
                if(pair.second == branch_type){
                    if(in_stack[pair.first]){
                        return true;
                    }
                    if(dfs(pair.first, branch_type, in_stack, diagram)){
                        return true;
                    }
                }
            }

            in_stack[node] = false;
            return false;
        }

        bool check_cycle(SharedNodePtr start,
                         BranchType branch_type,
                         const graph_type& diagram){
            std::unordered_map<SharedNodePtr,bool> in_stack;
            return dfs(start, branch_type, in_stack, diagram);
        }

        void paths_to_base(SharedNodePtr node,
                           const graph_type& diagram,
                           std::unordered_map<SharedNodePtr, int>& reach_count) {
            reach_count[node]++;
            auto it = diagram[node];
            for(const auto& [child, branchType] : it){
                if(branchType == BranchType::INHERITANCE){
                    paths_to_base(child, diagram, reach_count);
                }
            }
        }

        bool detect_diamond(SharedNodePtr start, const graph_type& diagram) {
            std::unordered_map<SharedNodePtr, int> counts;
            paths_to_base(start, diagram, counts);

            for(const auto &[_, count] : counts){
                if(count > 1){
                    return true;
                }
            }
            return false;
        }

        bool diamond(const graph_type& diagram) {
            std::unordered_set<SharedNodePtr> all_nodes;

            for(auto it = diagram.begin(); it != diagram.end(); it++){
                auto parent = it.key();
                auto children = it.value();
                all_nodes.insert(parent);
                for(const auto& [child, branchType] : children){
                    if(branchType == BranchType::INHERITANCE){
                        all_nodes.insert(child);
                    }
                }
            }

            for(SharedNodePtr start : all_nodes){
                if(detect_diamond(start, diagram)){
                    return true;
                }
            }
            return false;
        }

        bool validate_inheritance(std::string& errorMessage, SharedNodePtr child,
                                  SharedNodePtr parent,
                                  const graph_type& diagram){
            if(check_cycle(child, BranchType::INHERITANCE, diagram)){
                errorMessage = "Connection creates Cycle";
                return false;
            }

            if(diamond(diagram)){
                errorMessage = "Warning diamond problem";
                return false;
            }
            return true;
        }

        bool validate_others(std::string& errorMessage, BranchType branchType, SharedNodePtr child,
                             SharedNodePtr parent,
                             const graph_type& diagram){
            if(branchType == BranchType::COMPOSITION || branchType == BranchType::AGGREGATION){
                if(check_cycle(child, branchType, diagram)){
                    errorMessage = "Connection creates circular inheritance problem";
                    return false;
                }
            }
            return true;
        }
    }
    bool validate(std::string& errorMessage, SharedNodePtr child,
                  SharedNodePtr parent,
                  BranchType branchType,
                  const graph_type& diagram){
        if(child == parent){
            errorMessage = "Cannot self connection";
            return false;
        }
        bool result;

        switch (branchType) {
        case BranchType::INHERITANCE:
            result = validate_inheritance(errorMessage, child, parent, diagram);
            break;
        default:
            result = validate_others(errorMessage, branchType, child, parent, diagram);
        }
        return result;
    }

}


namespace testvalidator{
    bool check_multiple_connection(SharedNodePtr child, SharedNodePtr parent,
                                   const graph_type& diagram){
        auto value = diagram[child];
        int count = 0;
        for(auto& pair : value){
            if(pair.first == parent){
                count++;
            }
        }
        return count < 2;
    }

    bool detect_diamond(SharedNodePtr start, const graph_type& diagram) {
        std::unordered_map<SharedNodePtr, int> counts;
        paths_to_base(start, diagram, counts);

        for(const auto &[_, count] : counts){
            if(count > 1){
                return true;
            }
        }
        return false;
    }

    void paths_to_base(SharedNodePtr node,
                       const graph_type& diagram,
                       std::unordered_map<SharedNodePtr, int>& reach_count) {
        reach_count[node]++;
        auto it = diagram[node];
        for(const auto& [child, branchType] : it){
            if(branchType == BranchType::INHERITANCE){
                paths_to_base(child, diagram, reach_count);
            }
        }
    }

    bool check_cycle(SharedNodePtr start,
                     BranchType branch_type,
                     const graph_type& diagram){
        std::unordered_map<SharedNodePtr,bool> in_stack;
        return dfs(start, branch_type, in_stack, diagram);
    }

    bool dfs(SharedNodePtr node,
             BranchType branch_type,
             std::unordered_map<SharedNodePtr,bool>& in_stack,
             const graph_type& diagram){
        in_stack[node] = true;

        auto it = diagram[node];
        for(auto& pair : it){
            if(pair.second == branch_type){
                if(in_stack[pair.first]){
                    return true;
                }
                if(dfs(pair.first, branch_type, in_stack, diagram)){
                    return true;
                }
            }
        }

        in_stack[node] = false;
        return false;
    }

}

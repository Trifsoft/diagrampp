#include "validator.h"

#include <QDebug>
#include <string>


namespace Validator {

    static const std::map<std::pair<QString, QString>, bool> combinations = {
        {{"class", "class"}, true},
        {{"class", "struct"}, true},
        {{"class", "enum"}, false},
        {{"struct", "class"}, true},
        {{"struct", "struct"}, true},
        {{"struct", "enum"}, false},
        {{"enum", "class"}, true},
        {{"enum", "struct"}, true},
        {{"enum", "enum"}, false}
    };


    bool validate(std::string& errorMessage, SharedNodePtr child, SharedNodePtr parent, BranchType branchType, const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram){
        if(child == parent){
            errorMessage = "Cannot self connection";
            return false;
        }
        // proveri duplication u circularity
        if(!checkLinkage(errorMessage, child, parent)){
            return false;
        }

        bool result;
        switch (branchType){
            case BranchType::INHERITANCE:
                result = validateInheritance(errorMessage, child, parent, diagram);
                break;
            default:
                result = validateOthers(errorMessage, branchType, child, parent, diagram);
        }
        return result;
    }

    bool validateInheritance(std::string& errorMessage, SharedNodePtr child, SharedNodePtr parent, const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram){
        //if(isInterface(child) || isAbstract(child))

        if(hasCycle(child, BranchType::INHERITANCE, diagram)){
            errorMessage = "Connection creates Cycle";
            return false;
        }

        if(diamond(diagram)){
            errorMessage = "Warning diamond problem";
            return false;
        }
        return true;
    }
    bool validateOthers(std::string& errorMessage, BranchType branchType, SharedNodePtr child, SharedNodePtr parent, const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram){
        if(branchType == BranchType::COMPOSITION || branchType == BranchType::AGGREGATION){
            if(hasCycle(child, branchType, diagram)){
                errorMessage = "Connection creates circular inheritance problem";
                return false;
            }
        }
        return true;
    }

    bool checkLinkage(std::string& errorMessage, SharedNodePtr first, SharedNodePtr second){
        auto key = std::make_pair(first->get_uml_class_diagram_node()->get_label(), second->get_uml_class_diagram_node()->get_label());
        auto it = combinations.find(key);

        if(it != combinations.end()){
            if(it->second == false){
                errorMessage = "Linkage error";
            }
            return it->second;
        }
        qDebug() << "Unknown combination";
        return false;
    }

    bool dfs(SharedNodePtr node,
             BranchType branch_type,
             std::unordered_map<SharedNodePtr,bool>& in_stack,
             const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram){
        in_stack[node] = true;

        std::vector<std::pair<SharedNodePtr, BranchType>> value = diagram.at(node);
        for(auto& pair : value){
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
    bool hasCycle(SharedNodePtr start,
                  BranchType branch_type,
                  const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram){
        int diagram_size = diagram.size();
        std::unordered_map<SharedNodePtr,bool> in_stack;
        return dfs(start, branch_type, in_stack, diagram);
    }

    void paths_to_base(SharedNodePtr node,
                        const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram,
                        std::unordered_map<SharedNodePtr, int>& reach_count){

        reach_count[node]++;
        auto it = diagram.at(node);
        for(const auto& [child, branchType] : it){
            if(branchType == BranchType::INHERITANCE){
                paths_to_base(child, diagram, reach_count);
            }
        }
    }

    bool detect_diamond(SharedNodePtr start, const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram){
        std::unordered_map<SharedNodePtr, int> counts;

        paths_to_base(start, diagram, counts);

        bool has_issue = false;
        for(const auto &[_, count] : counts){
            if(count > 1){
                has_issue = true;
            }
        }

        return has_issue;
    }

    bool diamond(const std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram) {
        std::unordered_set<SharedNodePtr> all_nodes;

        for(const auto& [parent, children] : diagram){
            all_nodes.insert(parent);
            for(const auto& [child, branchType] : children){
                if(branchType == BranchType::INHERITANCE){
                    all_nodes.insert(child);
                }
            }
        }

        for(SharedNodePtr start : all_nodes){
            if (detect_diamond(start, diagram)) {
                return true;
            }
        }
        return false;
    }
}

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

    bool validateDiagram(std::string& errorMessage, NodeView* child, NodeView* parent, BranchType branchType, std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram){
        if(child == parent){
            errorMessage = "Cannot self connection";
            return false;
        }
        // proveri duplication u circularity
        if(!checkDuplication(errorMessage, child, parent, diagram) || !checkLinkage(errorMessage, child, parent)){
            return false;
        }

        // CAUTION: validator will not modify the diagram. move to board
        diagram[child].push_back({parent, branchType});

        bool result;
        switch (branchType){
            case BranchType::INHERITANCE:
                result = validateInheritance(errorMessage, child, parent, diagram);
                break;
            default:
                result = validateOthers(errorMessage, branchType, child, parent, diagram);
        }
        if(!result){
            diagram[child].pop_back();
            return result;
        }
        return result;
    }

    bool checkDuplication(std::string& errorMessage, NodeView* child, NodeView* parent, std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram){
        for(auto iterator : diagram[child]){
            if(parent == iterator.first){
                errorMessage = "Connections between nodes exists";
                return false;
            }
        }
        return true;
    }
    bool validateInheritance(std::string& errorMessage, NodeView* child, NodeView* parent, std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram){
        //diamond
        //if(isInterface(child) || isAbstract(child))
        std::set<NodeView*> stack;
        if(circularity(BranchType::INHERITANCE, stack, child, diagram)){
            errorMessage = "Connection creates circular inheritance problem";
            return false;
        }
        return true;
    }
    bool validateOthers(std::string& errorMessage, BranchType branchType, NodeView* child, NodeView* parent, std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram){
        if(branchType == BranchType::COMPOSITION || branchType == BranchType::AGGREGATION){
            std::set<NodeView*> stack;
            if(circularity(branchType, stack, child, diagram)){
                errorMessage = "Connection creates circular inheritance problem";
                return false;
            }
        }
        return true;
    }

    bool checkLinkage(std::string& errorMessage, NodeView* first, NodeView* second){
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

    // can optimize via parent?
    bool circularity(BranchType branchType, std::set<NodeView*>& stack, NodeView* node, std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>& diagram){
        if(stack.find(node) != stack.end()){
            return true;
        }

        stack.insert(node);
        std::vector<std::pair<SharedNodePtr, BranchType>> value = diagram[node];
        for(auto& pair : value){
            if(pair.second == branchType){
                if(circularity(branchType, stack, pair.first, diagram)){
                    return true;
                }
            }
        }
        return false;
    }


}

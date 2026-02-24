#ifndef BRANCHES_H
#define BRANCHES_H

#include <string>

enum class BranchType{
    INHERITANCE,
    ASSOCIATION,
    REALIZATION,
    AGGREGATION,
    COMPOSITION,
    DEPENDENCY
};


inline std::string get_string_from_branch_type(BranchType branch_type){
    switch(branch_type){
    case BranchType::INHERITANCE:
        return "Inheritance";
    case BranchType::ASSOCIATION:
        return "Association";
    case BranchType::REALIZATION:
            return "Realization";
    case BranchType::AGGREGATION:
        return "Aggregation";
    case BranchType::COMPOSITION:
        return "Composition";
    default: // BranchType::DEPENDENCY
        return "Dependency";
    }
}

inline BranchType get_branch_type_from_string(const std::string& s){
    if(s == "Inheritance"){
        return BranchType::INHERITANCE;
    }else if(s == "Association"){
        return BranchType::ASSOCIATION;
    }else if(s == "Realization"){
        return BranchType::REALIZATION;
    }else if(s == "Aggregation"){
        return BranchType::AGGREGATION;
    }else if(s == "Composition"){
        return BranchType::COMPOSITION;
    }

    // s == "Dependency"
    return BranchType::DEPENDENCY;
}

#endif // BRANCHES_H

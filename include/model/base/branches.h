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
        return "inheritance";
    case BranchType::ASSOCIATION:
        return "association";
    case BranchType::REALIZATION:
            return "realization";
    case BranchType::AGGREGATION:
        return "aggregation";
    case BranchType::COMPOSITION:
        return "composition";
    default: // BranchType::DEPENDENCY
        return "dependency";
    }
}

inline BranchType get_branch_type_from_string(const std::string& s){
    if(s == "inheritance"){
        return BranchType::INHERITANCE;
    }else if(s == "association"){
        return BranchType::ASSOCIATION;
    }else if(s == "realization"){
        return BranchType::REALIZATION;
    }else if(s == "aggregation"){
        return BranchType::AGGREGATION;
    }else if(s == "composition"){
        return BranchType::COMPOSITION;
    }

    // s == "dependency"
    return BranchType::DEPENDENCY;
}

#endif // BRANCHES_H

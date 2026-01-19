#include "validator.h"

#include <QDebug>
#include <string>

enum class CombinationType {
    ClassClass,
    ClassStruct,
    ClassEnum,
    StructClass,
    StructStruct,
    StructEnum,
    EnumClass,
    EnumEnum,
    EnumStruct,
    Unknown
};

CombinationType getCombinationType(const QString& label1, const QString& label2){
    QString combination = label1 + label2;

    if(combination == "classclass")     return CombinationType::ClassClass;
    if(combination == "classstruct")    return CombinationType::ClassStruct;
    if(combination == "enumclass")      return CombinationType::EnumClass;
    if(combination == "classenum")      return CombinationType::ClassEnum;
    if(combination == "structclass")    return CombinationType::StructClass;
    if(combination == "enumenum")       return CombinationType::EnumEnum;
    if(combination == "enumstruct")     return CombinationType::EnumStruct;
    if(combination == "structstruct")   return CombinationType::StructStruct;
    if(combination == "structenum")     return CombinationType::StructEnum;

    return CombinationType::Unknown;
}

Validator::Validator() {}



bool Validator::checkLinkage(std::shared_ptr<IUMLClassDiagramNode> first, std::shared_ptr<IUMLClassDiagramNode> second, BranchType branch){
    CombinationType type = getCombinationType(first.get()->get_label(), second.get()->get_label());
    auto cppclassfirst = dynamic_cast<CPPClass*>(first.get())->parent;
    auto cppclasssecond = dynamic_cast<CPPClass*>(second.get())->parent;
    switch(type){
        case CombinationType::ClassClass:{
            auto cppclassfirst = dynamic_cast<CPPClass*>(first.get())->parent;
            auto cppclasssecond = dynamic_cast<CPPClass*>(second.get())->parent;
            cppclassfirst->createConnection(cppclasssecond); // dodaj branch
            break;
        }
        case CombinationType::ClassStruct:{

        }
        case CombinationType::EnumClass:{
            break;
        }
        case CombinationType::ClassEnum:{
            break;
        }
        case CombinationType::StructClass:{
            return false;
        }
        case CombinationType::EnumEnum:{
            return false;
        }
        case CombinationType::EnumStruct:{

        }
        case CombinationType::StructStruct:{

        }
        case CombinationType::StructEnum:{

        }
        default:  // Correct spelling
            qDebug() << "Unknown combination type";
            break;
    }


    return false;
}





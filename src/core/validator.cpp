#include "validator.h"

#include <QDebug>
#include <string>

enum class CombinationType {
    ClassClass,
    ClassStruct,
    StructClass,
    EnumClass,
    ClassEnum,
    Unknown
};

CombinationType getCombinationType(const QString& label1, const QString& label2){
    QString combination = label1 + label2;

    if(combination == "classclass") return CombinationType::ClassClass;
    if(combination == "classstruct") return CombinationType::ClassStruct;
    if(combination == "enumclass") return CombinationType::EnumClass;
    if(combination == "classenum") return CombinationType::ClassEnum;
    if(combination == "structclass") return CombinationType::StructClass;

    return CombinationType::Unknown;
}

Validator::Validator() {}




// dodaj grane istrazi sutra.
// first moze biti IUMLCLASSDIAGRAMNODE to jest (enum)
bool Validator::checkLinkage(std::shared_ptr<IUMLClassDiagramNode> first, std::shared_ptr<IUMLClassDiagramNode> second, BranchType branch){
    CombinationType type = getCombinationType(first.get()->get_label(), second.get()->get_label());
    switch(type){
        case CombinationType::ClassClass:{

            // inherit, association, navigation
            // ako se u edit doda klasa onda dodati mada ne bih ovo radio.

            auto cppclassfirst = dynamic_cast<CPPClass*>(first.get())->parent;
            auto cppclasssecond = dynamic_cast<CPPClass*>(second.get())->parent;
            cppclassfirst->createConnection(cppclasssecond);
            break;
        }
        case CombinationType::StructClass:{

        }
        case CombinationType::ClassStruct:{
            break;
        }
        case CombinationType::EnumClass:{
            break;
        }
        case CombinationType::ClassEnum:{
            break;
        }
        default:  // Correct spelling
            qDebug() << "Unknown combination type";
            break;
    }


    return false;
}





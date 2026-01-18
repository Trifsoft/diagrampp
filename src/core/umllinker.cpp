#include "umllinker.h"

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

UMLLinker::UMLLinker() {}

void UMLLinker::checkLinkage(DiagramGraph* diagram, IUMLClassDiagramNode* activator, BranchType branch){
    IUMLClassDiagramNode* first = nullptr;
    IUMLClassDiagramNode* second = activator;

    // znaci ako imas enum ne moze biti prvi jebiga
    // istrazi malo sutra sta i kako i samo znaj da cppenum ne nasledjuje nista.
    for(std::shared_ptr<IUMLClassDiagramNode> iterator : diagram->nodes){
        IUMLClassDiagramNode* node = iterator.get();
        if(node->Clicked && node != activator){
            first = node;
            node->Clicked = false;
        }
    }

    if(first != nullptr){
        second->Clicked = false;
        doLinkage(diagram, first, second);
    }
}
// dodaj grane istrazi sutra.
// first moze biti IUMLCLASSDIAGRAMNODE to jest (enum)
bool UMLLinker::doLinkage(DiagramGraph* diagram, IUMLClassDiagramNode* first, IUMLClassDiagramNode* second){
    CombinationType type = getCombinationType(first->get_label(), second->get_label());
    switch(type){
        case CombinationType::ClassClass:{

            // inherit, association, navigation
            // ako se u edit doda klasa onda dodati mada ne bih ovo radio.

            auto cppclassfirst = dynamic_cast<CPPClass*>(first)->parent;
            auto cppclasssecond = dynamic_cast<CPPClass*>(second)->parent;
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
            break;    }

    if(first->get_label() != "enum" && second->get_label() != "enum"){
        // dodaj liniju.
        diagram->addLink(second->shared_from_this(), first->shared_from_this());
    }
    return false;
}





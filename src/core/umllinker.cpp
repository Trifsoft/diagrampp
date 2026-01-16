#include "umllinker.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_enum.h"
#include "model/elements/composition/cpp_struct.h"
#include <QDebug>

UMLLinker::UMLLinker() {}

void UMLLinker::checkLinkage(DiagramGraph* diagram, IUMLClassDiagramNode* activator){
    IUMLClassDiagramNode* first = nullptr;
    IUMLClassDiagramNode* second = activator;

    // znaci ako imas enum ne moze biti prvi jebiga
    // istrazi malo sutra sta i kako i samo znaj da cppenum ne nasledjuje nista.
    for(auto *nodes : diagram->nodes){
        if(nodes->Clicked && nodes != activator){
            first = nodes;
            nodes->Clicked = false;
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
    qDebug() << "uslo";
    if(first->get_label() != "enum" && second->get_label() != "enum"){
        // dodaj liniju.
        diagram->addLink(dynamic_cast<Composition*>(first), dynamic_cast<Composition*>(second));
    }
    return false;
}





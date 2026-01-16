#ifndef UMLLINKER_H
#define UMLLINKER_H

#include "graph/diagram_graph.h"
#include "model/base/uml_class_diagram_node.h"
#include "model/base/branchs.h"



class UMLLinker {
public:
    UMLLinker();
    void checkLinkage(DiagramGraph* diagram, IUMLClassDiagramNode* activator);
    bool doLinkage(DiagramGraph* diagram, IUMLClassDiagramNode* first, IUMLClassDiagramNode* second);
private:
    branchs branch;
};

#endif // UMLLINKER_H

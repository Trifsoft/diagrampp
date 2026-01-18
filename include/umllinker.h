#ifndef UMLLINKER_H
#define UMLLINKER_H

#include "graph/diagram_graph.h"
#include "model/base/uml_class_diagram_node.h"
#include "model/base/branches.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_enum.h"
#include "model/elements/composition/cpp_struct.h"



class UMLLinker {
public:
    UMLLinker();
    void checkLinkage(DiagramGraph* diagram, IUMLClassDiagramNode* activator, BranchType branch);
    bool doLinkage(DiagramGraph* diagram, IUMLClassDiagramNode* first, IUMLClassDiagramNode* second);
};

#endif // UMLLINKER_H

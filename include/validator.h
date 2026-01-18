#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "model/base/uml_class_diagram_node.h"
#include "model/base/branches.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_enum.h"
#include "model/elements/composition/cpp_struct.h"



class Validator {
public:
    Validator();
    bool checkLinkage(std::shared_ptr<IUMLClassDiagramNode> first, std::shared_ptr<IUMLClassDiagramNode> second, BranchType branch);

};

#endif // VALIDATOR_H

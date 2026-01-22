#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <view/cpp_class.h>
#include "model/base/branches.h"



class Validator {
public:
    Validator();
    //bool checkLinkage(std::shared_ptr<IUMLClassDiagramNode> first, std::shared_ptr<IUMLClassDiagramNode> second, BranchType branch);
    bool checkLinkage(NodeView* first, NodeView* second, BranchType branch);

};

#endif // VALIDATOR_H

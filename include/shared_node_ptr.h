#ifndef SHARED_NODE_PTR_H
#define SHARED_NODE_PTR_H

#include <memory>
#include <model/elements/composition/composition.h>

using Node = Composition;
using NodePtr = Node*;
using SharedNodePtr = std::shared_ptr<Node>;

#endif

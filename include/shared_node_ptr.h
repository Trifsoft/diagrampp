#ifndef SHARED_NODE_PTR_H
#define SHARED_NODE_PTR_H

#include <memory>
#include <model/elements/composition/composition.h>

using Node = Composition;
using NodeView = CppClassView;

using NodePtr = Node*;
using NodeViewPtr = NodeView*;

using SharedNodePtr = std::shared_ptr<Node>;

#endif

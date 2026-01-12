#include "graph/diagram_graph.h"

void DiagramGraph::addNode(IUMLClassDiagramNode* node) {
    if (!nodes.contains(node)) {
        nodes.append(node);
    }
}

void DiagramGraph::addLink(Composition* from, Composition* to) {
    links[from].append(to);
}

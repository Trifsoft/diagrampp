#include "graph/diagram_graph.h"

void DiagramGraph::addNode(std::shared_ptr<IUMLClassDiagramNode> node) {
    if(!node) return;

    if (!nodes.contains(node)) {
        nodes.append(node);
    }
}

void DiagramGraph::removeNode(std::shared_ptr<IUMLClassDiagramNode> node) {
    if(!node) return;

    nodes.removeAll(node);
}

void DiagramGraph::addLink(std::shared_ptr<IUMLClassDiagramNode> from, std::shared_ptr<IUMLClassDiagramNode> to, BranchType type) {
    links[from.get()].append({to, type});
}

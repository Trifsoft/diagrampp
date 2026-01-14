#include "graph/diagram_graph.h"

DiagramGraph::DiagramGraph(){
}

DiagramGraph::~DiagramGraph(){
    qDeleteAll(nodes);
    nodes.clear();

    for(auto &list : links) {
        qDeleteAll(list);
        list.clear();
    }
    qDeleteAll(links.keys());

    links.clear();
}

void DiagramGraph::addNode(IUMLClassDiagramNode* node) {
    if(!nodes.contains(node)) {
        nodes.append(node);
    }
}

void DiagramGraph::addLink(Composition* from, Composition* to) {
    links[from].append(to);
}

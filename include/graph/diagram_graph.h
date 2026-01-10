#ifndef DIAGRAM_GRAPH_H
#define DIAGRAM_GRAPH_H

#include <QList>
#include <QMap>

class IUMLClassDiagramNode;
class Composition;

class DiagramGraph {
public:
    QList<IUMLClassDiagramNode*> nodes;
    QMap<Composition*, QList<Composition*>> links;
    void addNode(IUMLClassDiagramNode* node);
    void addLink(Composition* from, Composition* to);
};

#endif // DIAGRAM_GRAPH_H

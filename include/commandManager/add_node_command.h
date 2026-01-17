#ifndef ADD_NODE_COMMAND_H
#define ADD_NODE_COMMAND_H

#include "command.h"
#include "graph/diagram_graph.h"
#include <QGraphicsScene>

class IUMLClassDiagramNode;

class AddNodeCommand : public Command
{
public:
    AddNodeCommand(QGraphicsScene* scene, DiagramGraph* graph, IUMLClassDiagramNode* node);
    
    void execute() override;
    void undo() override;
    
private:
    QGraphicsScene* scene;
    DiagramGraph* graph;
    IUMLClassDiagramNode* node;
    bool nodeAdded;
};

#endif // ADD_NODE_COMMAND_H

#ifndef ADD_NODE_COMMAND_H
#define ADD_NODE_COMMAND_H

#include "command.h"
#include "graph/diagram_graph.h"

class IUMLClassDiagramNode;

class AddNodeCommand : public Command
{
public:
    AddNodeCommand(DiagramGraph* graph, std::shared_ptr<IUMLClassDiagramNode> node);
    
    void execute() override;
    void undo() override;
    
private:
    DiagramGraph* m_graph;
    std::shared_ptr<IUMLClassDiagramNode> m_node;
    bool m_node_added;
};

#endif // ADD_NODE_COMMAND_H

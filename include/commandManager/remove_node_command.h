#ifndef REMOVE_NODE_COMMAND_H
#define REMOVE_NODE_COMMAND_H

#include "command.h"
#include "graph/diagram_graph.h"

class IUMLClassDiagramNode;

class RemoveNodeCommand : public Command
{
public:
    RemoveNodeCommand(DiagramGraph* graph,
                   SharedNodePtr node);

    void execute() override;
    void undo() override;

private:
    DiagramGraph* m_graph;
    SharedNodePtr m_node;
    bool m_node_removed;
};

#endif // REMOVE_NODE_COMMAND_H

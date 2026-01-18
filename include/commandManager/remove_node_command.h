#ifndef REMOVE_NODE_COMMAND_H
#define REMOVE_NODE_COMMAND_H

#include "command.h"
#include "graph/diagram_graph.h"

class IUMLClassDiagramNode;

class RemoveNodeCommand : public Command
{
public:
    RemoveNodeCommand(DiagramGraph* graph,
                   std::shared_ptr<IUMLClassDiagramNode> node);

    void execute() override;
    void undo() override;

private:
    DiagramGraph* m_graph;
    std::shared_ptr<IUMLClassDiagramNode> m_node;
    bool m_node_removed;
};

#endif // REMOVE_NODE_COMMAND_H

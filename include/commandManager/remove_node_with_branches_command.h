#ifndef REMOVE_NODE_WITH_BRANCHES_COMMAND_H
#define REMOVE_NODE_WITH_BRANCHES_COMMAND_H

#include "command.h"
#include "graph/diagram_graph.h"
#include "commandManager/composite_command.h"

class RemoveNodeWithBranchesCommand : public Command
{
public:
    RemoveNodeWithBranchesCommand(DiagramGraph* graph, SharedNodePtr node);

    void execute() override;
    void undo() override;

private:
    void build_commands();

    DiagramGraph* m_graph;
    SharedNodePtr m_node;
    bool m_executed;
    bool m_built;
    std::shared_ptr<CompositeCommand> m_composite;
};

#endif // REMOVE_NODE_WITH_BRANCHES_COMMAND_H

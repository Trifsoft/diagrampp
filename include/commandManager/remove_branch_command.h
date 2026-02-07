#ifndef REMOVE_BRANCH_COMMAND_H
#define REMOVE_BRANCH_COMMAND_H

#include "command.h"
#include "graph/diagram_graph.h"

class IUMLClassDiagramNode;

class RemoveBranchCommand : public Command
{
public:
    RemoveBranchCommand(DiagramGraph* graph,
                      SharedNodePtr from,
                      SharedNodePtr to,
                      BranchType branch_type);
    
    bool execute() override;
    bool undo() override;
    
private:
    DiagramGraph* m_graph;
    SharedNodePtr m_from;
    SharedNodePtr m_to;
    BranchType m_branch_type;
    bool m_branch_removed;
};

#endif // REMOVE_BRANCH_COMMAND_H

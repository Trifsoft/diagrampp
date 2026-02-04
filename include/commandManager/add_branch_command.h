#ifndef ADD_BRANCH_COMMAND_H
#define ADD_BRANCH_COMMAND_H

#include "command.h"
#include "graph/diagram_graph.h"

class IUMLClassDiagramNode;

class AddBranchCommand : public Command
{
public:
    AddBranchCommand(DiagramGraph* graph,
                   SharedNodePtr from,
                   SharedNodePtr to,
                   BranchType branch_type);
    
    void execute() override;
    void undo() override;
    
private:
    DiagramGraph* m_graph;
    SharedNodePtr m_from;
    SharedNodePtr m_to;
    BranchType m_branch_type;
    bool m_branch_added;
};

#endif // ADD_BRANCH_COMMAND_H

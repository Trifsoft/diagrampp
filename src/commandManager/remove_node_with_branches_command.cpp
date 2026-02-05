#include "commandManager/remove_node_with_branches_command.h"
#include "commandManager/remove_branch_command.h"
#include "commandManager/remove_node_command.h"

RemoveNodeWithBranchesCommand::RemoveNodeWithBranchesCommand(DiagramGraph* graph, SharedNodePtr node)
    : m_graph(graph), m_node(node), m_executed(false), m_built(false), m_composite(nullptr)
{
}

void RemoveNodeWithBranchesCommand::build_commands()
{
    m_composite = std::make_shared<CompositeCommand>();

    if (!m_graph || !m_node) {
        m_built = true;
        return;
    }

    auto branches = m_graph->get_branches_for_node(m_node);
    for (const auto& edge : branches) {
        auto remove_branch = std::make_shared<RemoveBranchCommand>(
            m_graph, edge.from, edge.to, edge.branch_type);
        m_composite->add_command(remove_branch);
    }

    auto remove_node = std::make_shared<RemoveNodeCommand>(m_graph, m_node);
    m_composite->add_command(remove_node);

    m_built = true;
}

void RemoveNodeWithBranchesCommand::execute()
{
    if (m_executed) {
        return;
    }

    if (!m_built) {
        build_commands();
    }

    if (m_composite) {
        m_composite->execute();
        m_executed = true;
    }
}

void RemoveNodeWithBranchesCommand::undo()
{
    if (!m_executed || !m_composite) {
        return;
    }

    m_composite->undo();
    m_executed = false;
}

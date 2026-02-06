#include "commandManager/add_branch_command.h"
#include <QDebug>


AddBranchCommand::AddBranchCommand(DiagramGraph* graph,
                               SharedNodePtr from,
                               SharedNodePtr to,
                               BranchType branch_type)
    : m_graph(graph), m_from(from), m_to(to), 
      m_branch_type(branch_type), m_branch_added(false)
{
}

bool AddBranchCommand::execute()
{
    m_graph->show_diagram();
    if (!m_branch_added && m_from && m_to && m_graph) {
        std::string error_msg;
        if(m_graph->add_branch(m_from, m_to, m_branch_type, error_msg)){
            m_branch_added = true;
            qDebug() << "Command EXECUTE: Branch dodat na scenu";
            return true;
        }else{
            qDebug() << error_msg;
        }
    }
    return false;
}

bool AddBranchCommand::undo()
{
    m_graph->show_diagram();

    if (m_branch_added && m_from && m_to && m_graph) {
        if(m_graph->remove_branch(m_from, m_to, m_branch_type)){
            m_branch_added = false;
            qDebug() << "Command UNDO: Branch uklonjen sa scene";
            return true;
        }else{
            qDebug() << "Command UNDO: Ne moze se ukloniti branch sa scene";
        }
    }
    return false;
}

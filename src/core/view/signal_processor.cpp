#include "view/signal_processor.h"

signalProcessor::signalProcessor(Board* board) : m_board(board) {}


void signalProcessor::add_link_process(SharedNodePtr from, SharedNodePtr to, BranchType branch) const {
    auto child = m_board->get_view_from_node(from);
    auto parent = m_board->get_view_from_node(to);
    if(child && parent){
        child->add_line_connection(parent, branch);
    }
}

void signalProcessor::remove_link_process(SharedNodePtr from, SharedNodePtr to, BranchType branch) const {
    auto child = m_board->get_view_from_node(from);
    auto parent = m_board->get_view_from_node(to);
    if(child && parent){
        child->remove_link_connection(parent, branch);
    }
}

void signalProcessor::remove_node_process(SharedNodePtr target) const {
    auto node = m_board->get_view_from_node(target);
    node->remove_node();
}


#include "view/signal_processor.h"

signalProcessor::signalProcessor() {}

void signalProcessor::add_link_process(CppClassView* child, CppClassView* parent, BranchType branch) const {
    if(child && parent){
        child->add_line_connection(parent, branch);
    }
}

void signalProcessor::remove_link_process(CppClassView* child, CppClassView* parent, BranchType branch) const {
    if(child && parent){
        child->remove_link_connection(parent, branch);
    }
}

void signalProcessor::remove_node_process(CppClassView* node) const {
    if(node){
        node->remove_node();
    }
}

void signalProcessor::add_node_process(CppClassView* node) const {
    if(node){
        node->add_node();
    }
}


#include "signal_processor.h"

signalProcessor::signalProcessor() {}


void signalProcessor::add_link_process(SharedNodePtr from, SharedNodePtr to, BranchType branch) const {
    CppClass* child = dynamic_cast<CppClass*>(from);
    CppClass* parent = dynamic_cast<CppClass*>(to);
    if(child && parent){
        child->add_line_connection(parent, branch);
    }
}

void signalProcessor::remove_link_process(SharedNodePtr from, SharedNodePtr to, BranchType branch) const {
    CppClass* child = dynamic_cast<CppClass*>(from);
    CppClass* parent = dynamic_cast<CppClass*>(to);
    if(child && parent){
        child->remove_link_connection(parent, branch);
    }
}

void signalProcessor::remove_node_process(SharedNodePtr target) const {
    CppClass* node = dynamic_cast<CppClass*>(target);
    node->remove_node();
}


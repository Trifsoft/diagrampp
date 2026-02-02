#include "signal_processor.h"

signalProcessor::signalProcessor() {}


void signalProcessor::add_link_process(SharedNodePtr from, SharedNodePtr to, BranchType branch) const {
    auto child = std::dynamic_pointer_cast<CppClass>(from);
    auto parent = std::dynamic_pointer_cast<CppClass>(to);
    // CppClass* child = dynamic_cast<CppClass*>(from); before change from NodeView* to std::shared_ptr<NodeView>
    // CppClass* parent = dynamic_cast<CppClass*>(to);
    if(child && parent){
        child->add_line_connection(parent.get(), branch);
    }
}

void signalProcessor::remove_link_process(SharedNodePtr from, SharedNodePtr to, BranchType branch) const {
    auto child = std::dynamic_pointer_cast<CppClass>(from);
    auto parent = std::dynamic_pointer_cast<CppClass>(to);
    if(child && parent){
        child->remove_link_connection(parent.get(), branch);
    }
}

void signalProcessor::remove_node_process(SharedNodePtr target) const {
    auto node = std::dynamic_pointer_cast<CppClass>(target);
    //CppClass* node = dynamic_cast<CppClass*>(target);
    node->remove_node();
}


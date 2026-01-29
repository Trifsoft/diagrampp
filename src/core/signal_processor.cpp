#include "signal_processor.h"

signalProcessor::signalProcessor() {}


void signalProcessor::add_link_process(SharedNodePtr from, SharedNodePtr to, BranchType branch){
    CppClass* child = dynamic_cast<CppClass*>(from);
    CppClass* parent = dynamic_cast<CppClass*>(to);
    if(child && parent){
        child->addLineConnection(parent, branch);
    }
}

void signalProcessor::remove_link_process(SharedNodePtr from, SharedNodePtr to, BranchType branch){
    CppClass* child = dynamic_cast<CppClass*>(from);
    CppClass* parent = dynamic_cast<CppClass*>(to);
    if(child && parent){
        child->removeLink(parent, branch);
    }

}

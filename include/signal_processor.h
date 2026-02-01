#ifndef SIGNAL_PROCESSOR_H
#define SIGNAL_PROCESSOR_H

#include "graph/diagram_graph.h"
#include "view/cpp_class.h"
#include "model/base/branches.h"
#include <QObject>


class signalProcessor : public QObject{
    Q_OBJECT
public:
    signalProcessor();

public slots:
    void add_link_process(SharedNodePtr from, SharedNodePtr to, BranchType branch) const;
    void remove_link_process(SharedNodePtr from, SharedNodePtr to, BranchType branch) const;
    void remove_node_process(SharedNodePtr node) const;

};

#endif // SIGNAL_PROCESSOR_H

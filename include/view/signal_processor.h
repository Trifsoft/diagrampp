#ifndef SIGNAL_PROCESSOR_H
#define SIGNAL_PROCESSOR_H

#include "graph/diagram_graph.h"
#include "view/cpp_class.h"
#include "Board.h"
#include "model/base/branches.h"
#include <QObject>


class Board;

class signalProcessor : public QObject{
    Q_OBJECT
public:
    signalProcessor(Board* board);

public slots:
    void add_link_process(SharedNodePtr from, SharedNodePtr to, BranchType branch) const;
    void remove_link_process(SharedNodePtr from, SharedNodePtr to, BranchType branch) const;
    void remove_node_process(SharedNodePtr node) const;
    void add_node_process(SharedNodePtr target) const;
private:
    Board* m_board;
};

#endif // SIGNAL_PROCESSOR_H

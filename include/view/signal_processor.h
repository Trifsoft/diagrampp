#ifndef SIGNAL_PROCESSOR_H
#define SIGNAL_PROCESSOR_H

#include "view/cpp_class_view.h"
#include "model/base/branches.h"
#include <QObject>


class Board;

class signalProcessor : public QObject{
    Q_OBJECT

public:
    signalProcessor(Board* board);

private:
    signalProcessor();
public:
    static signalProcessor& instance() {
        static signalProcessor s;
        return s;
    }
public slots:
    void add_link_process(CppClassView* child, CppClassView* parent, BranchType branch) const;
    void remove_link_process(CppClassView* child, CppClassView* parent, BranchType branch) const;
    void add_node_process(CppClassView* node) const;
    void remove_node_process(CppClassView* node) const;
};

#endif // SIGNAL_PROCESSOR_H

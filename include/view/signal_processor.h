#ifndef SIGNAL_PROCESSOR_H
#define SIGNAL_PROCESSOR_H

#include "view/cpp_class_view.h"
#include "model/base/branches.h"
#include <view/arrow.h>
#include <view/line.h>
#include <QObject>
#include <QMap>

using ConnectionInfoType = QMap<CppClassView*, QList<std::tuple<CppClassView*, CppClassView*, BranchType>>>;

class Connection: public QObject {
    Q_OBJECT
    Arrow* arrow;
    Line* line;
public:
    Connection(Arrow*, Line*);
    ~Connection();
};

class signalProcessor : public QObject{
    Q_OBJECT

private:
    QMap<std::tuple<CppClassView*, CppClassView*, BranchType>, Connection*> connections;
    ConnectionInfoType incomingConnectionInfo;
    ConnectionInfoType outgoingConnectionInfo;

    Arrow* get_arrow(CppClassView*, BranchType);
signals:
    void added_connection(Arrow*, Line*);
public slots:
    void add_link_process(CppClassView* child, CppClassView* parent, BranchType branch);
    void remove_link_process(CppClassView* child, CppClassView* parent, BranchType branch);
    void add_node_process(CppClassView* node);
    void remove_node_process(CppClassView* node);
public:
    signalProcessor();
    ~signalProcessor();
};

#endif // SIGNAL_PROCESSOR_H

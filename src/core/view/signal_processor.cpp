#include "view/signal_processor.h"

signalProcessor::~signalProcessor() {}
signalProcessor::signalProcessor() {}

Connection::Connection(Arrow* arrow, Line* line)
    : arrow(arrow)
    , line(line)
{
    arrow->setParent(this);
    line->setParent(this);
}

Connection::~Connection() {}

Arrow* signalProcessor::get_arrow(CppClassView* view, BranchType branch) {
    Arrow* new_arrow = new Arrow(branch, view->pos(), view->boundingRect().height(), incomingConnectionInfo[view].size());
    for(auto info : incomingConnectionInfo[view]) {
        auto connection = connections[info];
        connect(connection, &QObject::destroyed, new_arrow, &Arrow::move_back);
    }
    connect(view, &CppClassView::moved_by, new_arrow, &Arrow::move_by);
    connect(view, &CppClassView::height_changed_by, new_arrow, &Arrow::move_y);
    return new_arrow;
}

void signalProcessor::add_link_process(CppClassView* child, CppClassView* parent, BranchType branch) {
    if(child && parent){
        Arrow* arrow = get_arrow(parent, branch);

        Line* line = new Line(branch, QLineF(child->get_top_center(), arrow->get_bottom_center()));
        Connection* connection = new Connection(arrow, line);

        connect(arrow, &Arrow::moved_by, line, &Line::move_end);
        connect(child, &CppClassView::moved_by, line, &Line::move_start);

        connect(child, &QObject::destroyed, connection, &QObject::deleteLater);
        connect(parent, &QObject::destroyed, connection, &QObject::deleteLater);

        connections[{child, parent, branch}] = connection;
        outgoingConnectionInfo[child].append({child, parent, branch});
        incomingConnectionInfo[parent].append({child, parent, branch});

        emit added_connection(arrow, line);
    }
}

void signalProcessor::remove_link_process(CppClassView* child, CppClassView* parent, BranchType branch) {
    if(child && parent && connections.contains({child, parent, branch})){
        delete connections.take({child, parent, branch});
    }
}

void signalProcessor::remove_node_process(CppClassView* node) {
    if(node){
        delete node;
        if(incomingConnectionInfo.contains(node)) {
            auto info = incomingConnectionInfo.take(node);
            for(auto it = info.begin(); it != info.end(); it++) {
                connections.remove(*it);
            }
        }
        if(outgoingConnectionInfo.contains(node)) {
            auto info = incomingConnectionInfo.take(node);
            for(auto it = info.begin(); it != info.end(); it++) {
                connections.remove(*it);
            }
        }
    }
}

void signalProcessor::add_node_process(CppClassView* node) {
    if(node){

    }
}


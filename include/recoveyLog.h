#ifndef RECOVEYLOG_H
#define RECOVEYLOG_H

#include "QObject"
#include "graph/diagram_graph.h"
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <stdio.h>


class recoveryLog : public QObject{
    Q_OBJECT
public:
    recoveryLog(std::string _file_name = "recoveryLog.txt");
    ~recoveryLog();
public slots:
    void add_link_operation(SharedNodePtr from, SharedNodePtr to, BranchType BranchType);
    void remove_link_operation(SharedNodePtr from, SharedNodePtr to, BranchType BranchType);
    void remove_node_operation(SharedNodePtr node);

private:
    std::ofstream log_file;
    std::string file_name;
    unsigned OPERATION_WIDTH = 20;
    unsigned INFO_WIDTH      = 20;
    void write_log_header();
    std::string branch_to_string(BranchType branch_type);
    std::string get_current_timestamp();
    std::string get_username();

};

#endif // RECOVEYLOG_H

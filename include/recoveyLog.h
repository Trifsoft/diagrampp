#ifndef RECOVEYLOG_H
#define RECOVEYLOG_H

#include "QObject"
#include "graph/diagram_graph.h"
#include "model/elements/field.h"
#include "model/elements/method.h"
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <stdio.h>


class recoveryLog : public QObject{
    Q_OBJECT
public:
    recoveryLog(const std::string& _file_name = "recoveryLog.txt");
    ~recoveryLog();
public slots:
    void add_field_operation(Composition* node, std::shared_ptr<Field> field);
    void add_method_operation(Composition* node, std::shared_ptr<Method> method);
    void add_link_operation(SharedNodePtr from, SharedNodePtr to, BranchType BranchType);
    void remove_link_operation(SharedNodePtr from, SharedNodePtr to, BranchType BranchType);
    void remove_node_operation(SharedNodePtr node);


private:
    std::ofstream log_file;
    std::string file_name;
    int TIMESTAMP_WIDTH = 22;
    int OPERATION_WIDTH = 20;
    int USER_WIDTH = 15;
    int INFO_WIDTH = 50;
    int SEPARATOR_LENGTH = 80;
    void write_log_header();
    std::string branch_to_string(BranchType branch_type) const;
    std::string get_current_timestamp() const;
    std::string get_username() const;

};

#endif // RECOVEYLOG_H

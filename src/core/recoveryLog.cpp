#include "recoveyLog.h"
#include <ctime>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <model/elements/composition/composition.h>

recoveryLog::recoveryLog(const std::string& _file_name) : file_name(_file_name){
    if(std::filesystem::exists(file_name)){
        if(remove(file_name.c_str())){
            qDebug() << "Failed to delete file" << file_name;
        }
    }
    log_file = std::ofstream(file_name);
    if(!log_file.is_open()){
        std::cerr << "Failed to create recovery log" << std::endl;
        return;
    }
    write_log_header();
}

recoveryLog::~recoveryLog(){
    if(log_file.is_open()){
        log_file << "[" << get_current_timestamp() << "] "
                 << "[USER:" << get_username() << "] "
                 << "[SYSTEM] Log file closed\n";
        log_file.close();
    }
}

void recoveryLog::add_field_operation(Composition* node, std::shared_ptr<Field> field) {
    std::string node_name = node->get_name().toStdString();
    log_file << std::left
             << "[" << get_current_timestamp() << "]"
             << std::setw(OPERATION_WIDTH) << " [ADD FIELD] "
             << std::setw(INFO_WIDTH) << (node_name +
                                          " Field name: " + field->get_field_name().toStdString() +
                                          " Field type: " + field->get_type().toStdString() +
                                          " Field destructor " + field->get_destruction().value_or("doesn't have").toStdString()
                                          + "\n");
}
void recoveryLog::add_method_operation(Composition* node, std::shared_ptr<Method> method) {
    std::string node_name = node->get_name().toStdString();
    std::string arguments;
    for(auto& arg : method->get_arguments()){
        arguments.append("name : " + arg->get_name().toStdString() + " type : " + arg->get_type().toStdString() + " | ");
    }
    log_file << std::left
             << "[" << get_current_timestamp() << "]"
             << std::setw(OPERATION_WIDTH) << " [ADD METHOD] "
             << std::setw(INFO_WIDTH) << (node_name +
                                          " Method return type: " + method->get_return_type().toStdString() +
                                          " Method arguments " + arguments +
                                          "\n");

}


void recoveryLog::add_link_operation(SharedNodePtr from, SharedNodePtr to, BranchType branch_type) {
    std::string child = from->get_name().toStdString();
    std::string parent = to->get_name().toStdString();
    log_file << std::left
             << "[" << get_current_timestamp() << "]"
             << std::setw(OPERATION_WIDTH) << " [ADD LINK] "
             << std::setw(INFO_WIDTH) << (child + " → " + branch_to_string(branch_type) + " " + parent ) << "\n";
    log_file.flush();
}

void recoveryLog::remove_link_operation(SharedNodePtr from, SharedNodePtr to, BranchType branch_type) {
    std::string child = from->get_name().toStdString();
    std::string parent = to->get_name().toStdString();
    log_file << std::left
             << "[" << get_current_timestamp() << "]"
             << std::setw(OPERATION_WIDTH) << " [REMOVE LINK] "
             << std::setw(INFO_WIDTH) << (child + " → " + branch_to_string(branch_type) + " " + parent ) << "\n";
    log_file.flush();

}

void recoveryLog::remove_node_operation(SharedNodePtr target) {
    std::string node = target->get_name().toStdString();
    log_file << "[" << get_current_timestamp() << "]"
             << std::setw(OPERATION_WIDTH) << " [REMOVE NODE] "
             << std::setw(INFO_WIDTH) << node << "\n";
    log_file.flush();
}

std::string recoveryLog::get_current_timestamp() const {
    auto now = std::chrono::system_clock::now();
    auto in_time = std::chrono::system_clock::to_time_t(now);

    std::tm tm_buf;
    localtime_r(&in_time, &tm_buf);
    // format like: YYYY-MM-DD HH:MM:SS
    std::stringstream ss;
    ss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

std::string recoveryLog::get_username() const {
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if(pw){
        return std::string(pw->pw_name);
    }
#ifdef Q_OS_LINUX
    // if fails use ENV variables
    char* env_user = getenv("USER");
    if(env_user){
        return std::string(env_user);
    }
#endif
    return "unknown";
}


void recoveryLog::write_log_header() {
    log_file << "==========================================================\n";
    log_file << "RECOVERY LOG FILE\n";
    log_file << "Created: " << get_current_timestamp() << "\n";
    log_file << "User: " << get_username() << "\n";
    log_file << "PID: " << getpid() << "\n";
    log_file << "==========================================================\n\n";
    log_file.flush();
}

std::string recoveryLog::branch_to_string(BranchType branch_type) const {
    if(branch_type == BranchType::INHERITANCE){
        return std::string("INHERITANCE");
    }else if(branch_type == BranchType::REALIZATION){
        return std::string("REALIZATION");
    }else if(branch_type == BranchType::DEPENDENCY){
        return std::string("DEPENDENCY");
    }else if(branch_type == BranchType::COMPOSITION){
        return std::string("COMPOSITION");
    }else if(branch_type == BranchType::ASSOCIATION){
        return std::string("ASSOCIATION");
    }else if(branch_type == BranchType::AGGREGATION){
        return std::string("AGGREGATION");
    }
    return std::string("unkown");
}

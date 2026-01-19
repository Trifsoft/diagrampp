#include <filesystem>
#include <QFile>
#include "project_generator.h"

namespace fs = std::filesystem;

namespace {
    bool remove_existing_project(QString path){
        uintmax_t num_of_deletions = fs::remove_all(fs::path(path.toStdString()));

        return num_of_deletions > 0 ? true : false;
    }

    ProjectGenerator::GenerationStatusCode determine_error_code(std::error_code ec){
        if(ec == std::errc::permission_denied){
            return ProjectGenerator::GenerationStatusCode::PERMISSION_DENIED;
        }

        if(ec == std::errc::no_space_on_device){
            return ProjectGenerator::GenerationStatusCode::NO_MEMORY_SPACE;
        }

        return ProjectGenerator::GenerationStatusCode::OK;
    }
};

ProjectGenerator::GenerationStatusCode ProjectGenerator::generate(QString path, QString project_dir_name, bool replace_existing){
    namespace fs = std::filesystem;

    // ensures project_dir_name is string, NOT path
    project_dir_name.remove('/');
    project_dir_name.remove('\\');
    fs::path root_path = fs::path(path.toStdString()).append(project_dir_name.toStdString());

    if(fs::exists(root_path)){
        if(replace_existing){
            remove_existing_project(path);
        }else {
            return ProjectGenerator::GenerationStatusCode::EXISTING_DIR_ON_PATH;
        }
    }else{
        return ProjectGenerator::GenerationStatusCode::NO_SUCH_DIR;
    }

    std::error_code ec;
    if(!fs::create_directory(root_path, ec)){
        return determine_error_code(ec);
    }
    ec.clear();


    for (auto it = ProjectGenerator::dir_hierarchy.begin(), end = ProjectGenerator::dir_hierarchy.end(); it != end; it++) {
        fs::path root_directory = fs::path(root_path).append(it.key().toStdString());

        if(!fs::create_directory(root_directory, ec)){
            return determine_error_code(ec);
        }
        ec.clear();

        auto root_directory_subdirs = it.value();
        for(auto& subdir_name : root_directory_subdirs){
            fs::path subdir = fs::path(root_directory).append(subdir_name.toStdString());
            if(!create_directory(subdir, ec)){
                return determine_error_code(ec);
            }
            ec.clear();
        }
    }

    // generate declarations

    // generate definitions

    return ProjectGenerator::GenerationStatusCode::OK;
}

#include <algorithm>
#include "project_generator.h"
#include "graph/diagram_graph.h"
#include <unordered_set>
#include "model/base/uml_class_diagram_node.h"
#include <fstream>


namespace {
    bool remove_existing_project(const std::string& path){
        uintmax_t num_of_deletions = fs::remove_all(fs::path(path));

        return num_of_deletions > 0 ? true : false;
    }

    ProjectGenerator::GenerationStatusCode determine_error_code(const std::error_code& ec){
        if(ec == std::errc::permission_denied){
            return ProjectGenerator::GenerationStatusCode::PERMISSION_DENIED;
        }

        if(ec == std::errc::no_space_on_device){
            return ProjectGenerator::GenerationStatusCode::NO_MEMORY_SPACE;
        }

        return ProjectGenerator::GenerationStatusCode::OK;
    }

    const std::unordered_set<SharedNodePtr> retrieve_diagram_nodes(){
        const auto& diagram = DiagramGraph::diagram_graph().get_diagram();
        std::unordered_set<SharedNodePtr> nodes = {};

        for(const auto& it : diagram){
            nodes.insert(it.first);
            for(const auto [neighbour, _]: it.second){
                nodes.insert(neighbour);
            }
        }

        return nodes;
    }

    bool should_generate_cpp_file(SharedNodePtr node){
        // not done, waiting for class/interface/abstract  concretizations
        // determine should cpp file be generated for given node
        // should for:
        // class, struct
        // not for:
        // interface, abstract class

        return node.get()->definition().isEmpty(); // makes sense for now
    }
};

ProjectGenerator::GenerationStatusCode ProjectGenerator::generate(const std::string& path, std::string& project_dir_name, const bool replace_existing){
    namespace fs = std::filesystem;

    // ensures project_dir_name is string, NOT path
    project_dir_name.erase(std::remove(project_dir_name.begin(), project_dir_name.end(), '/'), project_dir_name.end());
    project_dir_name.erase(std::remove(project_dir_name.begin(), project_dir_name.end(), '\\'), project_dir_name.end());
    fs::path root_path = fs::path(path).append(project_dir_name);

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


    for (auto& it : ProjectGenerator::project_hierarchy) {
        fs::path root_directory = fs::path(root_path).append(it.first);

        if(!fs::create_directory(root_directory, ec)){
            return determine_error_code(ec);
        }
        ec.clear();

        auto root_directory_subdirs = it.second;
        for(auto& subdir_name : root_directory_subdirs){
            fs::path subdir = fs::path(root_directory).append(subdir_name);
            if(!create_directory(subdir, ec)){
                return determine_error_code(ec);
            }
            ec.clear();
        }
    }

    auto &nodes = retrieve_diagram_nodes();
    std::fstream file_stream;
    for (auto node : nodes){ // smart pointers are stored in nodes, auto& omitted on purpose
        auto& name = node->get_name(); // get name of struct/enum/class

        if(should_generate_cpp_file(node)){ // generate defnitions code for Struct/Class

            file_stream.open(fs::path(ProjectGenerator::file_location["cpp"].first + '/' + name + ".cpp", ProjectGenerator::file_location["cpp"].second), std::fstream::out);

            if(file_stream.is_open()){
                file_stream << node->definition().toStdString();
                file_stream.close();
            }else{
                return ProjectGenerator::GenerationStatusCode::NO_FILE_CREATED;
            }
        }

        file_stream.open(fs::path(ProjectGenerator::file_location["hpp"].first + '/' + name + ".hpp", ProjectGenerator::file_location["hpp"].second), std::fstream::out);

        if(file_stream.is_open()){
            file_stream << node->declaration().toStdString();
            file_stream.close();
        }else{
            return ProjectGenerator::GenerationStatusCode::NO_FILE_CREATED;
        }
    }


    return ProjectGenerator::GenerationStatusCode::OK;
}

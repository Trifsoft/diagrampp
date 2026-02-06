#include <algorithm>
#include "project_generator.h"
#include "graph/diagram_graph.h"
#include <unordered_set>
#include <model/elements/composition/composition.h>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace { // utlis
    void remove_existing_project(const fs::path& path){
        fs::remove_all(path); // recursively delete content in dir on path and dir on path itself
    }

    ProjectGenerator::GenerationStatusCode determine_error_code(const std::error_code& ec){
        if(ec == std::errc::permission_denied){
            return ProjectGenerator::GenerationStatusCode::PERMISSION_DENIED;
        }

        if(ec == std::errc::no_space_on_device){
            return ProjectGenerator::GenerationStatusCode::NO_MEMORY_SPACE;
        }

        return ProjectGenerator::GenerationStatusCode::FILE_NOT_CREATED;
    }

    const std::unordered_set<SharedNodePtr> retrieve_diagram_nodes(const graph_type& diagram){
        std::unordered_set<SharedNodePtr> nodes = {};

        for(const auto& it : diagram){
            nodes.insert(it.first);
            for(const auto& [neighbour, _]: it.second){
                nodes.insert(neighbour);
            }
        }

        return nodes;
    }

    bool should_generate_cpp_file(SharedNodePtr node){
        return !node->definition().isEmpty();
    }

    fs::path& append_n_times(fs::path& base_dir, std::vector<std::string>& dirs){
        for(auto& d : dirs){
            base_dir.append(d);
        }

        return base_dir;
    }

    const std::string format_file_name(const std::string& name, const ProjectGenerator::FileNameNotation notation){
        std::string file_name = "";
        file_name += notation == ProjectGenerator::FileNameNotation::CAMEL_NOTATION ? std::toupper(name[0]) : std::tolower(name[0]); // preamble

        for (int i = 1; i < name.size(); i++){
            if(std::isupper(name[i])){ // written in camel
                if(notation == ProjectGenerator::FileNameNotation::CAMEL_NOTATION){
                    file_name += name[i];
                }else{
                    file_name += '_';
                    file_name += std::tolower(name[i]);
                }
            }else if(name[i] == '_'){ // written in snake
                if(notation == ProjectGenerator::FileNameNotation::CAMEL_NOTATION){
                    i++;
                    if(i < name.size())
                        file_name += std::toupper(name[i]);
                }else{
                    file_name += '_';
                }
            }else {
                file_name += name[i];
            }
        }
        return file_name;
    }

    ProjectGenerator::GenerationStatusCode generate_cmake(const fs::path& path, std::string_view content){
        std::fstream file_stream;
        file_stream.open(fs::path(path).append("CMakeLists.txt"), std::fstream::out);

        if(file_stream.is_open()){
            file_stream << content;
            file_stream.close();
        }else{
            return ProjectGenerator::GenerationStatusCode::FILE_NOT_CREATED;
        }

        return ProjectGenerator::GenerationStatusCode::OK;
    }

    ProjectGenerator::GenerationStatusCode generate_dir_hierarchy(const fs::path& root_path, std::error_code& ec){
        auto gen_cmake_status = generate_cmake(root_path, ProjectGenerator::root_cmake);
        if(gen_cmake_status != ProjectGenerator::GenerationStatusCode::OK){
            return gen_cmake_status;
        }

        std::fstream file_stream;
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

                gen_cmake_status = generate_cmake(subdir, ProjectGenerator::subdir_cmake);
                if(gen_cmake_status != ProjectGenerator::GenerationStatusCode::OK){
                    return gen_cmake_status;
                }
            }
        }

        return ProjectGenerator::OK;
    }


    ProjectGenerator::GenerationStatusCode generate_files(const graph_type& diagram, const fs::path& root_path, const ProjectGenerator::FileNameNotation notation){
        auto &nodes = retrieve_diagram_nodes(diagram);
        std::fstream file_stream;
        for (auto& node : nodes){
            const auto& name = format_file_name(node->get_name().toStdString(), notation); // get name of struct/enum/class

            if(should_generate_cpp_file(node)){ // generate defnitions code for Struct/Class
                fs::path cpp_file_path = fs::path(root_path);
                cpp_file_path = append_n_times(cpp_file_path, ProjectGenerator::file_location["cpp"]);
                file_stream.open(cpp_file_path.append(name+".cpp"), std::fstream::out);

                fs::permissions(root_path, fs::perms::all, fs::perm_options::add);
                // debugged: problem with permission, file is not created on given path
                // if is never evaluated as true -> files are not created
                if(file_stream.is_open()){
                    file_stream << node->definition().toStdString();
                    file_stream.close();
                }else{
                    return ProjectGenerator::GenerationStatusCode::FILE_NOT_CREATED;
                }
            }

            fs::path hpp_file_path = fs::path(root_path);
            hpp_file_path = append_n_times(hpp_file_path, ProjectGenerator::file_location["hpp"]);
            file_stream.open(hpp_file_path.append(name+".hpp"), std::fstream::out);

            if(file_stream.is_open()){
                file_stream << node->declaration().toStdString();
                file_stream.close();
            }else{
                return ProjectGenerator::GenerationStatusCode::FILE_NOT_CREATED;
            }
        }
        return ProjectGenerator::OK;
    }
};

ProjectGenerator::GenerationStatusCode ProjectGenerator::generate(const graph_type& diagram, const std::string& path, std::string& project_dir_name, const ProjectGenerator::FileNameNotation notation, const ProjectGenerator::ReplaceToggle replace_mode){
    namespace fs = std::filesystem;
    // ensures project_dir_name is string, NOT path string
    project_dir_name.erase(std::remove(project_dir_name.begin(), project_dir_name.end(), '/'), project_dir_name.end());
    project_dir_name.erase(std::remove(project_dir_name.begin(), project_dir_name.end(), '\\'), project_dir_name.end());
    fs::path root_path = fs::path(path);

    if(!fs::exists(root_path) || !fs::is_directory(root_path)){
        return ProjectGenerator::GenerationStatusCode::NO_SUCH_DIR;
    }

    root_path.append(project_dir_name);

    if(fs::exists(root_path)){
        if(replace_mode == ProjectGenerator::ReplaceToggle::OFF){
            return ProjectGenerator::GenerationStatusCode::EXISTING_PROJECT_DIR_ON_PATH;
        }else{
            remove_existing_project(root_path);
        }
    }

    std::error_code ec;
    if(!fs::create_directory(root_path, ec)){
        return determine_error_code(ec);
    }
    ec.clear();


    auto res = generate_dir_hierarchy(root_path, ec);
    if(res != ProjectGenerator::OK){
        return res;
    }
    ec.clear();

    res = generate_files(diagram, root_path, notation);
    if(res != ProjectGenerator::OK){
        return res;
    }
    ec.clear();

    return ProjectGenerator::GenerationStatusCode::OK;
}

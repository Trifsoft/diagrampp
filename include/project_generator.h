#ifndef PROJECT_GENERATOR_H
#define PROJECT_GENERATOR_H

#include <map>
#include <vector>
#include <filesystem>
#include <model/base/branches.h>
#include <graph/diagram_graph.h>

class IUMLClassDiagramNode;

class ProjectGenerator: public QObject
{
public:
    enum GenerationStatusCode{
        OK,
        EXISTING_PROJECT_DIR_ON_PATH,
        PERMISSION_DENIED,
        NO_MEMORY_SPACE,
        FILE_NOT_CREATED,
        NO_SUCH_DIR
    };

    enum FileNameNotation{
        SNAKE_NOTATION,
        CAMEL_NOTATION
    };

    enum ReplaceToggle{
        ON,
        OFF
    };
    static ProjectGenerator& instance() {
        static ProjectGenerator inst;
        return inst;
    }
private:
    static constexpr std::string_view root_cmake = R"(
cmake_minimum_required(VERSION 3.16)
project(List VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra")

set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O2")

add_subdirectory(src/core)
        )";

    static constexpr std::string_view subdir_cmake = R"(
add_library()
target_include_directories()
add_executable()
target_link_libraries()
        )";


    // definition goes globally everytime included because of header files. (fixed with inline)
    static std::map<std::string, std::vector<std::string>>& project_hierarchy() {
        static std::map<std::string, std::vector<std::string>> hierarchy = {
            {"include", {}},
            {"src", {"core", "utils"}},
        };
        return hierarchy;
    }

    static std::map<std::string, std::vector<std::string>>& file_location() {
        static std::map<std::string, std::vector<std::string>> location = {
           {"hpp", {"include"}},
           {"cpp", {"src", "core"}},
        };
        return location;
    }

    ProjectGenerator();

    static void remove_existing_project(const std::filesystem::path& path);
    static GenerationStatusCode determine_error_code(const std::error_code& ec);
    static bool should_generate_cpp_file(SharedNodePtr node);
    static std::filesystem::path& append_n_times(std::filesystem::path& base_dir, std::vector<std::string>& dirs);
    static const std::string format_file_name(const std::string& name, const FileNameNotation notation);
    static const std::unordered_set<SharedNodePtr> retrieve_diagram_nodes(const graph_type& diagram);
    GenerationStatusCode generate_cmake(const std::filesystem::path& path, std::string_view content);
    GenerationStatusCode generate_dir_hierarchy(const std::filesystem::path& root_path, std::error_code& ec);
    GenerationStatusCode generate_files(const graph_type& diagram,
                                        const std::filesystem::path& root_path,
                                        const FileNameNotation notation);
public slots:
    GenerationStatusCode generate(const graph_type& diagram,
                                  const QString& path,
                                  QString& project_dir_name,
                                  const FileNameNotation notation,
                                  const ReplaceToggle replace_mode);
};

#endif // PROJECT_GENERATOR_H

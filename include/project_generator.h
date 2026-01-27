#ifndef PROJECT_GENERATOR_H
#define PROJECT_GENERATOR_H

#include <string>
#include <map>
#include <vector>
#include <model/base/branches.h>
#include <graph/diagram_graph.h>

class IUMLClassDiagramNode;

using graph_type = std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>>;

namespace ProjectGenerator
{
    std::map<std::string, std::vector<std::string>> project_hierarchy = {
        {"include", {}},
        {"src", {"core", "utils"}},
    };

    std::map<std::string, std::vector<std::string>> file_location = {
        {"hpp", {"include"}},
        {"cpp", {"src", "core"}},
    };

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

    GenerationStatusCode generate(const graph_type& diagram,  const std::string& path, std::string& project_dir_name, const FileNameNotation notation, const ReplaceToggle replace_mode);
};

#endif // PROJECT_GENERATOR_H

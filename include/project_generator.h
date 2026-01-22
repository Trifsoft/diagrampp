#ifndef PROJECT_GENERATOR_H
#define PROJECT_GENERATOR_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <model/base/branches.h>

class IUMLClassDiagramNode;

using graph_type = std::map<std::shared_ptr<IUMLClassDiagramNode>, std::vector<std::pair<std::shared_ptr<IUMLClassDiagramNode>, BranchType>>>;

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
        NO_FILE_CREATED,
        NO_SUCH_DIR
    };

    enum FileNameNotation{
        SNAKE_NOTATION,
        CAMEL_NOTATION
    };

    GenerationStatusCode generate(const graph_type& diagram,  const std::string& path, std::string& project_dir_name, FileNameNotation notation, const bool replace_existing);
};

#endif // PROJECT_GENERATOR_H

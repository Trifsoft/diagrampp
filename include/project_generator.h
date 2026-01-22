#ifndef PROJECT_GENERATOR_H
#define PROJECT_GENERATOR_H

#include <string>
#include <map>
#include <vector>

namespace ProjectGenerator
{
    std::map<std::string, std::vector<std::string>> project_hierarchy = {
        {"include", {}},
        {"src", {"core", "utils"}},
    };

    std::map<std::string, std::vector<std::string>> file_location = {
        {"include", {"include"}},
        {"src", {"core", "utils"}},
    };

    enum GenerationStatusCode{
        OK,
        EXISTING_DIR_ON_PATH,
        PERMISSION_DENIED,
        NO_MEMORY_SPACE,
        NO_FILE_CREATED,
        NO_SUCH_DIR
    };

    GenerationStatusCode generate(const std::string& path, std::string& project_dir_name, const bool replace_existing);
};

#endif // PROJECT_GENERATOR_H

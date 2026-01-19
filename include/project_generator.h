#ifndef PROJECT_GENERATOR_H
#define PROJECT_GENERATOR_H

#include <QString>
#include <assert.h>
#include <QMap>
#include <QList>


namespace ProjectGenerator
{
    QMap<QString, QList<QString>> dir_hierarchy = {
        {"include", {}},
        {"src", {"core", "utils"}},
    };

    QMap<QString, QString> file_location = {
        {".hpp", "include"},
        {".cpp", "src/core"},
    };

    enum GenerationStatusCode{
        OK,
        EXISTING_DIR_ON_PATH,
        PERMISSION_DENIED,
        NO_MEMORY_SPACE,
        NO_SUCH_DIR
    };

    GenerationStatusCode generate(QString path, QString root_project_dir_name, bool replace_existing);
};

#endif // PROJECT_GENERATOR_H

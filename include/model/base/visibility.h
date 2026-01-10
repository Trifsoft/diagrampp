#ifndef VISIBILITY_H
#define VISIBILITY_H

#include <QString>

enum class Visibility {
    Private,
    Protected,
    Public
};

QString get_declaration(Visibility);

#endif // VISIBILITY_H

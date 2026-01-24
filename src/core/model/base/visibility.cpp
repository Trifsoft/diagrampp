#include "model/base/visibility.h"

QString get_declaration(Visibility visibility) {
    switch(visibility) {
        case(Visibility::Private) : return "private";
        case(Visibility::Public) : return "public";
        case(Visibility::Protected) : return "protected";
    }
    return "";
}

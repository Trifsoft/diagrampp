#include "model/elements/type/regular_type.h"

RegularType::RegularType(const QString& name) : name(name) {}

QString RegularType::get_name() const {
    return name;
}

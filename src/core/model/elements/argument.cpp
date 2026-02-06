#include "model/elements/argument.h"

Argument::Argument(const QString& name, const QString& type)
    : name(name), type(type){
}

QString Argument::get_name() const {
    return name;
}

QString Argument::get_type() const {
    return type;
}

QString Argument::to_string() const {
    return type + " " + name;
}

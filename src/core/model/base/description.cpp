#include "model/base/description.h"

Description::Description(const QString& name, const std::shared_ptr<IType>& type, Reference reference, bool is_const)
    : name(name), type(type), reference(reference), is_const(is_const) {
}

QString Description::get_name() const {
    return name;
}

std::shared_ptr<IType> Description::get_type() const {
    return type;
}

Reference Description::get_reference() const {
    return reference;
}

bool Description::get_is_const() const {
    return is_const;
}

QString Description::to_string() const {
    QString result;

    if (is_const) {
        result += "const ";
    }

    result += type->get_name();
    result += get_symbol(reference);
    result += " ";
    result += name;

    return result;
}

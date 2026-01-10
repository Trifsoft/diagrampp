#include "model/elements/cpp_enum_value.h"

CPPEnumValue::CPPEnumValue(const QString& name, std::optional<int> value)
    : name(name), value(value) {}

QString CPPEnumValue::get_name() const {
    return name;
}

std::optional<int> CPPEnumValue::get_value() const {
    return value;
}

QString CPPEnumValue::to_string() const {
    if (!value.has_value()) {
        return name;
    } else {
        return name + " = " + QString::number(value.value());
    }
}

#include "model/elements/composition/cpp_enum.h"

CPPEnum::CPPEnum(const QString& name) : name(name) {}

QString CPPEnum::declaration() const {
    QString decl = "enum class " + name + " {\n";

    QStringList value_strings;
    for (const auto& value : values) {
        value_strings.append("\t" + value->to_string());
    }

    decl += value_strings.join(",\n");
    decl += "\n};";

    return decl;
}

QString CPPEnum::definition() const {
    return "";
}

void CPPEnum::add_value(const QString& name, std::optional<int> value) {
    values.append(new CPPEnumValue(name, value));
}

QString CPPEnum::get_name() const {
    return name;
}

QList<CPPEnumValue*> CPPEnum::get_values() const {
    return values;
}

QString CPPEnum::get_label() const {
    return "enum";
}


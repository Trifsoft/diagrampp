#include "model/elements/method.h"

Method::Method(const Description& description, Visibility visibility, MethodType method_type, const QList<Description>& variables)
    : IClassElement(ElementRank::Method, description.get_name(), visibility),
      description(description),
      method_type(method_type),
      variables(variables),
      definition_block("") {}

QString Method::get_base_method_declaration() const {
    QStringList var_strings;
    for (const auto& var : variables) {
        var_strings.append(var.to_string());
    }
    return description.to_string() + "(" + var_strings.join(", ") + ")";
}

QString Method::get_declaration() const {
    QString base = get_base_method_declaration();
    switch (method_type) {
        case MethodType::Regular:
            return base;
        case MethodType::RegularVirtual:
            return "virtual " + base;
        case MethodType::PureVirtual:
            return "virtual " + base + " = 0";
    }
    return base;
}

std::optional<QString> Method::definition(const QString& class_name) const {
    if (method_type == MethodType::PureVirtual) {
        return std::nullopt;
    }

    QStringList var_strings;
    for (const auto& var : variables) {
        var_strings.append(var.to_string());
    }

    QString def = description.get_type()->get_name() + " " + class_name + "::" +
                  description.get_name() + "(" + var_strings.join(", ") + ") {\n" +
                  definition_block + "\n}";
    return def;
}

Description Method::get_description() const {
    return description;
}

MethodType Method::get_method_type() const {
    return method_type;
}

QList<Description> Method::get_variables() const {
    return variables;
}

QString Method::get_definition_block() const {
    return definition_block;
}

void Method::set_definition_block(const QString& definition_block) {
    this->definition_block = definition_block;
}

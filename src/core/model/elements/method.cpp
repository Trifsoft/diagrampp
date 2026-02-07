#include "model/elements/method.h"

Method::Method(const QString& name, const QString& return_type, Visibility visibility, MethodKind method_kind, const QList<Argument*>& arguments, const QString& definition_block)
    : IClassElement(ElementRank::Method, name, visibility),
      return_type(return_type),
      method_kind(method_kind),
      arguments(arguments),
      definition_block(definition_block) {}

QString Method::get_call() const {
    QStringList var_strings;
    for (const auto& var : arguments) {
        var_strings.append(var->to_string());
    }
    return name + "(" + var_strings.join(", ") + ")";
}

QString Method::get_base_method_declaration() const {
    return return_type + " " + get_call();
}

QString Method::declaration() const {
    QString base = get_base_method_declaration();
    switch (method_kind) {
        case MethodKind::Regular:
            return base;
        case MethodKind::RegularVirtual:
            return "virtual " + base;
        case MethodKind::PureVirtual:
            return "virtual " + base + " = 0";
    }
    return base;
}

std::optional<QString> Method::definition(const QString& class_name) const {
    if (method_kind == MethodKind::PureVirtual) {
        return std::nullopt;
    }

    QStringList var_strings;
    for (const auto& var : arguments) {
        var_strings.append(var->to_string());
    }

    QString def = return_type + " " + class_name + "::" +
                  get_call() + "\n" +
                  "{\n" + definition_block + "\n}";
    return def;
}

QString Method::get_return_type() const
{
    return return_type;
}

MethodKind Method::get_method_kind() const {
    return method_kind;
}

QList<Argument*> Method::get_arguments() const {
    return arguments;
}

QString Method::get_definition_block() const {
    return definition_block;
}

void Method::set_definition_block(const QString& definition_block) {
    this->definition_block = definition_block;
}

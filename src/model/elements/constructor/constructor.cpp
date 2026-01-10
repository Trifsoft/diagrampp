#include "model/elements/constructor/constructor.h"

Constructor::Constructor(const QString& class_name, const QVector<Description>& arguments, Visibility visibility)
    : IClassElement(ElementRank::Constructor, "", visibility),
      class_name(class_name),
      arguments(arguments) {}

QString Constructor::get_declaration() const {
    QStringList arg_strings;
    for (const auto& arg : arguments) {
        arg_strings.append(arg.to_string());
    }
    return class_name + "(" + arg_strings.join(", ") + ")";
}

std::optional<QString> Constructor::definition(const QString& class_name) const {
    QStringList arg_strings;
    for (const auto& arg : arguments) {
        arg_strings.append(arg.to_string());
    }
    return class_name + "::" + class_name + "(" + arg_strings.join(", ") + ") : " + get_custom_definition();
}

QVector<Description> Constructor::get_arguments() const {
    return arguments;
}

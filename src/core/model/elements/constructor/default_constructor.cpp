#include "model/elements/constructor/default_constructor.h"

DefaultConstructor::DefaultConstructor(const QString& class_name, const QVector<Argument>& arguments, Visibility visibility)
    : Constructor(class_name, QVector<Argument>{}, visibility) {}

QString DefaultConstructor::get_custom_definition() const {
    QStringList init_list;
    for (const auto& arg : arguments) {
        init_list.append(arg->get_name() + "(" + arg->get_name() + ")");
    }
    return init_list.join(", ");
}

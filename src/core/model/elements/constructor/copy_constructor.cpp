#include "model/elements/constructor/copy_constructor.h"

CopyConstructor::CopyConstructor(const QString& class_name, const QList<std::shared_ptr<Field>>& class_fields, Visibility visibility)
    : Constructor(class_name, {Argument("other", "const " + class_name + "&")},
                  visibility),
      class_fields(class_fields) {}

QString CopyConstructor::get_custom_definition() const {
    QStringList init_list;
    for (const auto& field : class_fields) {
        init_list.append(field->get_name() + "(other." + field->get_name() + ")");
    }
    return init_list.join(", ");
}

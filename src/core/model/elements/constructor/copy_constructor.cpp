#include "model/elements/constructor/copy_constructor.h"
#include "model/elements/type/regular_type.h"
#include "model/base/reference.h"
#include <memory>

CopyConstructor::CopyConstructor(const QString& class_name, const QVector<Description*>& class_fields, Visibility visibility)
    : Constructor(class_name,
                  QVector<Description>{Description("other", std::make_shared<RegularType>(class_name), Reference::LValue, true)},
                  visibility),
      class_fields(class_fields) {}

QString CopyConstructor::get_custom_definition() const {
    QStringList init_list;
    for (const auto& field : class_fields) {
        init_list.append(field->get_name() + "(other." + field->get_name() + ")");
    }
    return init_list.join(", ");
}

#include "model/elements/destructor.h"
#include "model/elements/field.h"
#include "model/elements/type/regular_type.h"
#include "model/base/description.h"
#include <memory>

Destructor::Destructor(const QString& class_name, const QVector<std::shared_ptr<Field>>& fields, bool is_virtual)
    : IClassElement(ElementRank::Destructor, "", Visibility::Public),
      class_name(class_name),
      fields(fields),
      is_virtual(is_virtual) {}

bool Destructor::is_default() const {
    for (const auto& field : fields) {
        if (field->get_destruction().has_value()) {
            return false;
        }
    }
    return true;
}

QString Destructor::get_definition_block() const {
    QStringList destructions;
    for (const auto& field : fields) {
        if (field->get_destruction().has_value()) {
            destructions.append(field->get_destruction().value());
        }
    }
    return destructions.join("\n");
}

QString Destructor::get_declaration() const {
    QString decl;
    if (is_virtual) {
        decl += "virtual ";
    }
    decl += "~" + class_name + "()";
    if (is_default()) {
        decl += " = default";
    }
    return decl;
}

std::optional<QString> Destructor::definition(const QString& class_name) const {
    if (is_default()) {
        return std::nullopt;
    }

    QString def = "~" + class_name + "::" + class_name + "() {\n" +
                  get_definition_block() + "\n}";
    return def;
}

bool Destructor::get_is_virtual() const {
    return is_virtual;
}

#include "model/elements/field.h"

/*
Field::Field(const Description& description, Visibility visibility)
    : IClassElement(ElementRank::Field, description.get_name(), visibility),
      description(description),
      destruction(std::nullopt) {}
*/


Field::Field(QString name, QString type, Visibility visibility, std::optional<QString> destruction)
    : IClassElement(ElementRank::Field, name, visibility),
    type(type),
    destruction(destruction) {}

// QString Field::get_declaration() const {
//     return description.to_string();
// }

QString Field::get_declaration() const {
    return type + "  "  + get_name();
}

QString Field::get_type() const
{
    return type;
}


std::optional<QString> Field::definition(const QString& class_name) const {
    (void)class_name;
    return std::nullopt;
}

Description Field::get_description() const {
    return description;
}

void Field::set_destruction(const std::optional<QString>& destruction) {
    this->destruction = destruction;
}

void Field::set_destruction(const QString& destruction) {
    this->destruction = destruction;
}

std::optional<QString> Field::get_destruction() const {
    return destruction;
}

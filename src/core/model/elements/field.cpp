#include "model/elements/field.h"

Field::Field(QString name, QString type, Visibility visibility, std::optional<QString> destruction)
    : IClassElement(ElementRank::Field, name, visibility),
      type(type),
      destruction(destruction) {}

QString Field::declaration() const {
    return type + " : "  + name;
}



std::optional<QString> Field::definition(const QString& class_name) const {
    (void)class_name;
    return std::nullopt;
}

void Field::set_destruction(const std::optional<QString>& destruction) {
    this->destruction = destruction;
}

void Field::set_destruction(const QString& destruction) {
    this->destruction = destruction;
}

QString Field::get_type() const
{
    return type;
}

std::optional<QString> Field::get_destruction() const {
    return destruction;
}

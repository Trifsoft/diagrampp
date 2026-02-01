#include "model/elements/field.h"

Field::Field(std::shared_ptr<Description> description, Visibility visibility)
    : Field(description, visibility, std::nullopt) {}

Field::Field(std::shared_ptr<Description> description, Visibility visibility, std::optional<QString> destruction)
    : IClassElement(ElementRank::Field, description->get_name(), visibility),
      description(description),
      destruction(destruction) {}

QString Field::get_declaration() const {
    return description->to_string();
}

std::optional<QString> Field::definition(const QString& class_name) const {
    (void)class_name;
    return std::nullopt;
}

Description* Field::get_description() const {
    return description.get();
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

#ifndef FIELD_H
#define FIELD_H

#include <model/base/class_element.h>
#include <model/base/description.h>
#include <model/base/visibility.h>
#include <QString>
#include <optional>


//Will be refactored to remove type system
class Field : public IClassElement {
private:

    // TO BE REMOVED
    Description description;

    QString type;

    std::optional<QString> destruction;

public:
    // Field(const Description& description, Visibility visibility);
    Field(QString name, QString type, Visibility visibility, std::optional<QString> destruction = std::nullopt);
    Field(const Field& other) = default;

    QString get_declaration() const override;
    QString get_type() const;
    std::optional<QString> definition(const QString& class_name) const override;

    Description get_description() const;
    void set_destruction(const std::optional<QString>& destruction);
    void set_destruction(const QString& destruction);
    std::optional<QString> get_destruction() const;
};

#endif // FIELD_H

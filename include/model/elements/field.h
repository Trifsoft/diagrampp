#ifndef FIELD_H
#define FIELD_H

#include <model/elements/class_element.h>
#include <model/base/visibility.h>
#include <QString>
#include <optional>


//Refactored to remove type system and description
class Field : public IClassElement {
private:

    QString type;
    std::optional<QString> destruction;

public:
    // Field(const Description& description, Visibility visibility);
    Field(QString name, QString type, Visibility visibility, std::optional<QString> destruction = std::nullopt);

    QString declaration() const override;
    std::optional<QString> definition(const QString& class_name) const override;

    void set_destruction(const std::optional<QString>& destruction);
    void set_destruction(const QString& destruction);
    QString get_type() const;
    std::optional<QString> get_destruction() const;
};

#endif // FIELD_H

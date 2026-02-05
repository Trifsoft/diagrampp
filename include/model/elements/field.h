#ifndef FIELD_H
#define FIELD_H

#include <model/elements/class_element.h>
#include <model/base/visibility.h>
#include <QString>
#include <optional>
#include <memory>


//Refactored to remove type system and description
class Field : public IClassElement {
private:
    QString type;
    std::optional<QString> destruction;
    QString m_name;
public:
    Field(QString name, QString type, Visibility visibility, std::optional<QString> destruction = std::nullopt);

    QString declaration() const override;
    std::optional<QString> definition(const QString& class_name) const override;

    void set_destruction(const std::optional<QString>& destruction);
    void set_destruction(const QString& destruction);
    QString get_type() const;
    QString get_field_name() const;
    std::optional<QString> get_destruction() const;
};

#endif // FIELD_H

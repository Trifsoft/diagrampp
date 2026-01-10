#ifndef DESTRUCTOR_H
#define DESTRUCTOR_H

#include <model/base/class_element.h>
#include <model/base/visibility.h>
#include <QString>
#include <QVector>
#include <optional>

class Field;

class Destructor : public IClassElement {
private:
    QString class_name;
    QVector<Field> fields;
    bool is_virtual;

    bool is_default() const;
    QString get_definition_block() const;

public:
    Destructor(const QString& class_name, const QVector<Field>& fields, bool is_virtual = false);

    QString get_declaration() const override;
    std::optional<QString> definition(const QString& class_name) const override;

    bool get_is_virtual() const;
};

#endif // DESTRUCTOR_H

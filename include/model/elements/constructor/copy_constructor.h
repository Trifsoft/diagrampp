#ifndef COPY_CONSTRUCTOR_H
#define COPY_CONSTRUCTOR_H

#include "constructor.h"
#include <model/elements/argument.h>
#include <model/elements/field.h>
#include <QString>
#include <QVector>

class CopyConstructor : public Constructor {
private:
    QVector<Field> class_fields;

protected:
    QString get_custom_definition() const override;

public:
    CopyConstructor(const QString& class_name, const QVector<Field>& class_fields, Visibility visibility);
    CopyConstructor(const CopyConstructor& other) = default;
};

#endif // COPY_CONSTRUCTOR_H

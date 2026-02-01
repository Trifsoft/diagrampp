#ifndef COPY_CONSTRUCTOR_H
#define COPY_CONSTRUCTOR_H

#include "constructor.h"
#include <model/base/description.h>
#include <model/base/visibility.h>
#include <model/elements/field.h>
#include <QString>
#include <QVector>

class CopyConstructor : public Constructor {
private:
    QList<std::shared_ptr<Field>> class_fields;

protected:
    QString get_custom_definition() const override;

public:
    CopyConstructor(const QString& class_name, const QList<std::shared_ptr<Field>>& class_fields, Visibility visibility);
};

#endif // COPY_CONSTRUCTOR_H

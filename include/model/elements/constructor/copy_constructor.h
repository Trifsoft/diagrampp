#ifndef COPY_CONSTRUCTOR_H
#define COPY_CONSTRUCTOR_H

#include "constructor.h"
#include <model/base/visibility.h>
#include <model/elements/argument.h>
#include <model/elements/field.h>
#include <QString>
#include <QVector>


// [TODO] Add initialization for base class if there is inheritance
// [REFACTOR] class_fields should somehow reference fields in composition class (not just copy them)
// in order to avoid storing old data when fields change.
class CopyConstructor : public Constructor {
private:
    QList<std::shared_ptr<Field>> class_fields;


protected:
    QString get_custom_definition() const override;

public:
    CopyConstructor(const QString& class_name, const QList<std::shared_ptr<Field>>& class_fields, Visibility visibility);
};

#endif // COPY_CONSTRUCTOR_H

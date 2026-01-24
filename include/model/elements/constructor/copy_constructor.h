#ifndef COPY_CONSTRUCTOR_H
#define COPY_CONSTRUCTOR_H

#include "constructor.h"
#include <model/base/description.h>
#include <model/base/visibility.h>
#include <QString>
#include <QVector>

class CopyConstructor : public Constructor {
private:
    QVector<Description> class_fields;

protected:
    QString get_custom_definition() const override;

public:
    CopyConstructor(const QString& class_name, const QVector<Description>& class_fields, Visibility visibility);
};

#endif // COPY_CONSTRUCTOR_H

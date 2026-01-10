#ifndef DEFAULT_CONSTRUCTOR_H
#define DEFAULT_CONSTRUCTOR_H

#include "constructor.h"
#include <model/base/description.h>
#include <model/base/visibility.h>
#include <QString>
#include <QVector>

class DefaultConstructor : public Constructor {
protected:
    QString get_custom_definition() const override;

public:
    DefaultConstructor(const QString& class_name, const QVector<Description>& arguments, Visibility visibility);
};

#endif // DEFAULT_CONSTRUCTOR_H

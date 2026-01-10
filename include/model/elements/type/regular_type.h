#ifndef REGULAR_TYPE_H
#define REGULAR_TYPE_H

#include "type.h"
#include <QString>

class RegularType : public IType {
private:
    QString name;

public:
    explicit RegularType(const QString& name);
    ~RegularType() override = default;

    QString get_name() const override;
};

#endif // REGULAR_TYPE_H

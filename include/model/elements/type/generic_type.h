#ifndef GENERIC_TYPE_H
#define GENERIC_TYPE_H

#include "type.h"
#include <QString>
#include <memory>

class GenericType : public IType {
private:
    std::unique_ptr<IType> outer_type;
    std::unique_ptr<IType> inner_type;

public:
    GenericType(std::unique_ptr<IType> outer_type, std::unique_ptr<IType> inner_type);
    ~GenericType() override = default;

    QString get_name() const override;
};

#endif // GENERIC_TYPE_H

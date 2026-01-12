#include "model/elements/type/generic_type.h"

GenericType::GenericType(std::unique_ptr<IType> outer_type, std::unique_ptr<IType> inner_type)
    : outer_type(std::move(outer_type)), inner_type(std::move(inner_type)) {}

QString GenericType::get_name() const {
    return outer_type->get_name() + "<" + inner_type->get_name() + ">";
}

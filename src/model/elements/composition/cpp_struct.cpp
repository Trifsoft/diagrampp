#include "model/elements/composition/cpp_struct.h"

CPPStruct::CPPStruct(const QString& name, Visibility visibility, std::optional<std::pair<Visibility, Composition*>> inheritance)
    : Composition(name, visibility, inheritance) {}

Visibility CPPStruct::get_default_visibility() const {
    return Visibility::Public;
}

QString CPPStruct::get_label() const {
    return "struct";
}

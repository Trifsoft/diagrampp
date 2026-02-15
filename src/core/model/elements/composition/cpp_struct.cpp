#include "model/elements/composition/cpp_struct.h"

CPPStruct::CPPStruct(const QString& name, Visibility visibility, std::optional<std::pair<Visibility, Composition*>> inheritance)
    : Composition(name, visibility, inheritance) {}

NodeType CPPStruct::getNodeType() const
{
    return NodeType::Struct;
}

Visibility CPPStruct::get_default_visibility() const {
    return Visibility::Public;
}

QString CPPStruct::get_label() const {
    return "struct";
}

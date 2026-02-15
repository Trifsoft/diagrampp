#include "model/elements/composition/cpp_class.h"

CPPClass::CPPClass(const QString& name, Visibility visibility, std::optional<std::pair<Visibility, Composition*>> inheritance)
    : Composition(name, visibility, inheritance) {}

NodeType CPPClass::getNodeType() const
{
    return NodeType::Class;
}

Visibility CPPClass::get_default_visibility() const {
    return Visibility::Private;
}

QString CPPClass::get_label() const {
    return "class";
}

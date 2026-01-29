#include "model/elements/class_element.h"

int IClassElement::s_next_id = 0;

IClassElement::IClassElement(ElementRank rank, const QString& name, Visibility vis)
    : element_rank(rank), element_id(s_next_id++), name(name), visibility(vis) {
}

IClassElement::~IClassElement() {
}

const Visibility IClassElement::get_visibility() const
{
    return visibility;
}

const QString IClassElement::get_name() const
{
    return name;
}

const ElementRank IClassElement::get_rank() const
{
    return element_rank;
}

int IClassElement::get_id() const
{
    return element_id;
}

int IClassElement::compare_to(const IClassElement& other) const {
    // Note: First check for Method subclass comparison is skipped
    // Will need to be implemented in Method subclass if needed

    if (visibility != other.visibility) {
        return static_cast<int>(visibility) - static_cast<int>(other.visibility);
    }

    if (element_rank != other.element_rank) {
        return static_cast<int>(element_rank) - static_cast<int>(other.element_rank);
    }

    return name.compare(other.name);
}

bool IClassElement::operator<(const IClassElement& other) const {
    return compare_to(other) < 0;
}

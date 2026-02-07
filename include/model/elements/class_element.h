#ifndef CLASS_ELEMENT_H
#define CLASS_ELEMENT_H

#include <QString>
#include <optional>
#include <model/base/visibility.h>

enum class ElementRank {
    Constructor,
    Destructor,
    Field,
    Method,
};

class IClassElement {
protected:  
    ElementRank element_rank;
    QString name;
    Visibility visibility;

public:
    // IClassElement(ElementRank rank, const QString& name, Visibility vis);
    IClassElement(ElementRank rank, const QString& name, Visibility vis);
    virtual ~IClassElement();

    const ElementRank get_rank() const;
    const QString get_name() const;
    const Visibility get_visibility() const;

    virtual QString declaration() const = 0;
    virtual std::optional<QString> definition(const QString& class_name) const = 0;

    int compare_to(const IClassElement& other) const;
    bool operator<(const IClassElement& other) const;
};

#endif // CLASS_ELEMENT_H

#ifndef CPP_CLASS_H
#define CPP_CLASS_H

#include "composition.h"
#include <model/base/visibility.h>
#include <QString>
#include <optional>

class CppClassView;

class CPPClass : public Composition {
protected:
    Visibility get_default_visibility() const override;
    QString get_label() const override;
public:
    CPPClass(const QString& name, Visibility visibility = Visibility::Public, std::optional<std::pair<Visibility, Composition*>> inheritance = std::nullopt);
    NodeType getNodeType() const override;
};

#endif // CPP_CLASS_H

#ifndef CPP_STRUCT_H
#define CPP_STRUCT_H

#include "composition.h"
#include <model/base/visibility.h>
#include <QString>
#include <optional>

class CppClass;


class CPPStruct : public Composition {
protected:
    Visibility get_default_visibility() const override;
    CPPStruct::NodeLabel get_label() const override;
public:
    CPPStruct(const QString& name, Visibility visibility = Visibility::Public, std::optional<std::pair<Visibility, Composition*>> inheritance = std::nullopt);
};

#endif // CPP_STRUCT_H

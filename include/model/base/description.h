#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <memory>
#include <QString>
#include <model/base/reference.h>
#include <model/elements/type/type.h>

class Description {
private:
    QString name;
    std::shared_ptr<IType> type;
    Reference reference;
    bool is_const;
public:
    Description(const QString& name, const std::shared_ptr<IType>& type, Reference reference = Reference::None, bool is_const = false);

    QString get_name() const;
    std::shared_ptr<IType> get_type() const;
    Reference get_reference() const;
    bool get_is_const() const;

    QString to_string() const;
};

#endif // DESCRIPTION_H

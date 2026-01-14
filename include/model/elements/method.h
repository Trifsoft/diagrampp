#ifndef METHOD_H
#define METHOD_H

#include <model/base/class_element.h>
#include <model/base/description.h>
#include <model/base/visibility.h>
#include <QString>
#include <QVector>
#include <optional>

enum class MethodType {
    Regular,
    RegularVirtual,
    PureVirtual
};

class Method : public IClassElement {
private:
    Description description;
    MethodType method_type;
    QList<Description> variables;
    QString definition_block;

    QString get_base_method_declaration() const;

public:
    Method(const Description& description, Visibility visibility, MethodType method_type, const QList<Description>& variables);

    QString get_declaration() const override;
    std::optional<QString> definition(const QString& class_name) const override;

    Description get_description() const;
    MethodType get_method_type() const;
    QList<Description> get_variables() const;
    QString get_definition_block() const;
    void set_definition_block(const QString& definition_block);
};

#endif // METHOD_H

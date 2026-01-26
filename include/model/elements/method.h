#ifndef METHOD_H
#define METHOD_H

#include <model/elements/class_element.h>
#include <model/elements/argument.h>
#include <model/base/visibility.h>
#include <QString>
#include <QVector>
#include <optional>

enum class MethodKind {
    Regular,
    RegularVirtual,
    PureVirtual
};

class Method : public IClassElement {
private:
    QString return_type;
    MethodKind method_kind;
    QList<Argument> arguments;

    QString definition_block;
    QString get_base_method_declaration() const;

public:
    // Method(const Argument& description, Visibility visibility, MethodKind method_kind, const QList<Argument>& arguments);
    Method(const QString& name, const QString& return_type, Visibility visibility, MethodKind method_kind, const QList<Argument>& arguments);
    Method(const Method& other) = default;

    QString declaration() const override;
    std::optional<QString> definition(const QString& class_name) const override;

    QString get_return_type() const;
    MethodKind get_method_kind() const;
    QList<Argument> get_arguments() const;
    QString get_definition_block() const;
    void set_definition_block(const QString& definition_block);
};

#endif // METHOD_H

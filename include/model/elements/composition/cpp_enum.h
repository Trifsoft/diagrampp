#ifndef CPP_ENUM_H
#define CPP_ENUM_H

#include <model/base/uml_class_diagram_node.h>
#include <model/elements/cpp_enum_value.h>
#include <QString>
#include <QVector>
#include <optional>

class CppClass;


class CPPEnum : public IUMLClassDiagramNode {
private:
    QString name;
    QVector<CPPEnumValue> values;

public:
    explicit CPPEnum(const QString& name);

    QString declaration() const override;
    QString definition() const override;

    void add_value(const QString& name, std::optional<int> value = std::nullopt);
    QString get_name() const override;
    QVector<CPPEnumValue> get_values() const;
    NodeLabel get_label() const override;

};

#endif // CPP_ENUM_H

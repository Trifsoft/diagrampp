#ifndef COMPOSITION_H
#define COMPOSITION_H

#include <model/base/uml_class_diagram_node.h>
#include <model/base/visibility.h>
#include <model/elements/field.h>
#include <model/elements/method.h>
#include <model/elements/constructor/constructor.h>
#include <model/elements/destructor.h>
#include <QString>
#include <QVector>
#include <QMap>
#include <optional>
#include <memory>

class Composition : public IUMLClassDiagramNode {
protected:
    QString name;
    Visibility visibility;
    std::optional<std::pair<Visibility, Composition*>> inheritance;
    QVector<std::shared_ptr<Constructor>> constructors;

    virtual Visibility get_default_visibility() const = 0;

    Destructor get_destructor() const;
    QVector<IClassElement*> get_new_code_elements();
    QVector<const IClassElement*> get_new_code_elements() const;

public:

    // [REFACTOR] use shared_ptr for fields and methods, useful with command manager and copy constructor for example
    QVector<Field> fields;
    QVector<Method> methods;

    Composition(const QString& name, Visibility visibility, std::optional<std::pair<Visibility, Composition*>> inheritance = std::nullopt);
    virtual ~Composition() = default;

    QString declaration() const override;
    QString definition() const override;
    QString get_name() const override;

    QVector<const IClassElement*> get_code_elements() const;
    QMap<Visibility, QVector<const IClassElement*>> get_grouped_code_elements() const;

    void add_field(const QString& name, const QString& type, std::optional<Visibility> visibility = std::nullopt);
    void add_field(const Field& field);
    void add_method(const QString& name, const QString& type, Visibility visibility, MethodKind method_type, const QVector<Argument>& variables);
    void add_method(const Method& method);
    void add_constructor(Visibility visibility, const QVector<Argument>& arguments);
    void add_copy_constructor(Visibility visibility);

    int get_field_count();
    int get_method_count();
};

#endif // COMPOSITION_H

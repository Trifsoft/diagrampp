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
    QVector<std::shared_ptr<Field>> fields;
    QVector<std::shared_ptr<Method>> methods;

    Composition(const QString& name, Visibility visibility, std::optional<std::pair<Visibility, Composition*>> inheritance = std::nullopt);
    virtual ~Composition() = default;

    QString declaration() const override;
    QString definition() const override;
    QString get_name() const override;

    QVector<const IClassElement*> get_code_elements() const;
    QMap<Visibility, QVector<const IClassElement*>> get_grouped_code_elements() const;

    void add_field(const QString& name, std::shared_ptr<IType> type, std::optional<Visibility> visibility = std::nullopt);
    void add_method(const QString& name, std::shared_ptr<IType> type, Visibility visibility, MethodType method_type, const QVector<Description>& variables);
    void add_constructor(Visibility visibility, const QVector<Description>& arguments);
    void add_copy_constructor(Visibility visibility);

    int get_field_count();
    int get_method_count();
};

#endif // COMPOSITION_H

#ifndef COMPOSITION_H
#define COMPOSITION_H

#include <model/base/uml_class_diagram_node.h>
#include <model/base/visibility.h>
#include <model/elements/field.h>
#include <model/elements/method.h>
#include <model/elements/constructor/constructor.h>
#include <model/elements/constructor/copy_constructor.h>
#include <model/elements/constructor/default_constructor.h>
#include <model/elements/destructor.h>
#include <QString>
#include <QVector>
#include <QMap>
#include <optional>
#include <memory>

class CppClass;

class Composition : public IUMLClassDiagramNode {
    Q_OBJECT
protected:
    QString name;
    Visibility visibility;
    std::optional<std::pair<Visibility, Composition*>> inheritance;

    virtual Visibility get_default_visibility() const = 0;

    QVector<IClassElement*> get_new_code_elements();
    QVector<const IClassElement*> get_new_code_elements() const;

    std::optional<Visibility> copy_constructor_visibility;

signals:
    void changed();

public:

    Destructor get_destructor() const;
    std::shared_ptr<CopyConstructor> get_copy_constructor() const;

    QVector<std::shared_ptr<DefaultConstructor>> constructors;
    QVector<std::shared_ptr<Field>> fields;
    QVector<std::shared_ptr<Method>> methods;

    Composition(const QString& name, Visibility visibility, std::optional<std::pair<Visibility, Composition*>> inheritance = std::nullopt);
    virtual ~Composition() = default;

    QString declaration() const override;
    QString definition() const override;
    QString get_name() const override;
    void set_name(const QString&);

    QVector<const IClassElement*> get_code_elements() const;
    QMap<Visibility, QVector<const IClassElement*>> get_grouped_code_elements() const;

    void add_field(const QString& name, const QString& type, std::optional<Visibility> visibility = std::nullopt);
    void add_field(std::shared_ptr<Field> field);
    
    void add_method(const QString& name, const QString& type, MethodKind method_type, const QVector<Argument*>& variables, std::optional<Visibility> visibility = std::nullopt);
    void add_method(std::shared_ptr<Method> method);
    
    void add_constructor(const QVector<Argument*>& arguments, std::optional<Visibility> visibility = std::nullopt);
    void add_constructor(std::shared_ptr<DefaultConstructor>);

    void add_copy_constructor(std::optional<Visibility> visibility = std::nullopt);
    void remove_copy_constructor();

    int get_field_count();
    int get_method_count();

};

#endif // COMPOSITION_H

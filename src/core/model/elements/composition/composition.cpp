#include "model/elements/composition/composition.h"
#include "model/elements/constructor/default_constructor.h"
#include "model/elements/constructor/copy_constructor.h"
#include "model/base/description.h"
#include <algorithm>

Composition::Composition(const QString& name, Visibility visibility, std::optional<std::pair<Visibility, Composition*>> inheritance)
    : name(name), visibility(visibility), inheritance(inheritance) {}

Destructor Composition::get_destructor() const {
    return Destructor(name, fields);
}

QVector<IClassElement*> Composition::get_new_code_elements() {
    QVector<IClassElement*> elements;
    for (auto& field : fields) {
        elements.append(&field);
    }
    for (auto& method : methods) {
        elements.append(&method);
    }
    for (auto& constructor : constructors) {
        elements.append(constructor.get());
    }
    return elements;
}

QVector<const IClassElement*> Composition::get_new_code_elements() const {
    QVector<const IClassElement*> elements;
    for (const auto& field : fields) {
        elements.append(&field);
    }
    for (const auto& method : methods) {
        elements.append(&method);
    }
    for (const auto& constructor : constructors) {
        elements.append(constructor.get());
    }
    Destructor* destructor = new Destructor(get_destructor());
    elements.append(destructor);
    return elements;
}

QVector<const IClassElement*> Composition::get_code_elements() const {
    QVector<const IClassElement*> elements = get_new_code_elements();

    if (inheritance.has_value()) {
        auto inherited_elements = inheritance.value().second->get_code_elements();
        for (const auto* elem : inherited_elements) {
            const Method* method = dynamic_cast<const Method*>(elem);
            if (method && method->get_method_type() != MethodType::Regular) {
                elements.append(elem);
            }
        }
    }

    std::sort(elements.begin(), elements.end(), [](const IClassElement* a, const IClassElement* b) {
        return *a < *b;
    });

    return elements;
}

QMap<Visibility, QVector<const IClassElement*>> Composition::get_grouped_code_elements() const {
    QMap<Visibility, QVector<const IClassElement*>> grouped;
    auto elements = get_code_elements();

    for (const auto* elem : elements) {
        Visibility vis = Visibility::Public;
        grouped[vis].append(elem);
    }

    return grouped;
}

QString Composition::declaration() const {
    QString decl = get_declaration(visibility) + " " + get_label() + " " + name;

    if (inheritance.has_value()) {
        auto [vis, base_class] = inheritance.value();
        decl += " : " + get_declaration(vis) + " " + base_class->get_name();
    }

    decl += " {";

    auto grouped = get_grouped_code_elements();
    for (auto it = grouped.begin(); it != grouped.end(); ++it) {
        Visibility vis = it.key();
        const auto& elements = it.value();

        decl += "\n" + get_declaration(vis) + ":\n";
        for (const auto* elem : elements) {
            decl += "\t" + elem->get_declaration() + ";\n";
        }
    }

    decl += "};";
    return decl;
}

QString Composition::definition() const {
    QStringList definitions;
    auto elements = get_code_elements();

    for (const auto* elem : elements) {
        auto def = elem->definition(name);
        if (def.has_value()) {
            definitions.append(def.value());
        }
    }

    return definitions.join("\n\n");
}

void Composition::add_field(const QString& name, std::shared_ptr<IType> type, std::optional<Visibility> visibility) {
    Visibility vis = visibility.value_or(get_default_visibility());
    fields.append(Field(Description(name, type), vis));
}

void Composition::add_method(const QString& name, std::shared_ptr<IType> type, Visibility visibility, MethodType method_type, const QList<Description>& variables) {
    methods.append(Method(Description(name, type), visibility, method_type, variables));
}

void Composition::add_constructor(Visibility visibility, const QVector<Description>& arguments) {
    constructors.append(std::make_shared<DefaultConstructor>(name, arguments, visibility));
}

void Composition::add_copy_constructor(Visibility visibility) {
    QVector<Description> field_descriptions;
    for (const auto& field : fields) {
        field_descriptions.append(field.get_description());
    }
    constructors.append(std::make_shared<CopyConstructor>(name, field_descriptions, visibility));
}

QString Composition::get_name() const {
    return name;
}

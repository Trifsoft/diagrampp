#include <model/elements/composition/composition.h>
#include "model/elements/constructor/default_constructor.h"
#include "model/elements/constructor/copy_constructor.h"
#include "model/elements/argument.h"
#include <algorithm>

Composition::Composition(const QString& name, Visibility visibility, std::optional<std::pair<Visibility, Composition*>> inheritance)
    : name(name), visibility(visibility), inheritance(inheritance) {}

Destructor Composition::get_destructor() const {
    return Destructor(name, fields);
}

std::shared_ptr<CopyConstructor> Composition::get_copy_constructor() const {
    if(copy_constructor_visibility.has_value()) {
        return std::make_shared<CopyConstructor>(name, fields, copy_constructor_visibility.value());
    }
    else {
        return nullptr;
    }
}


// [REFACTOR] returning references, dangerous if vector gets reallocated !!!
QVector<IClassElement*> Composition::get_new_code_elements() {
    QVector<IClassElement*> elements;
    for (auto& field : fields) {
        elements.append(field.get());
    }
    for (auto& method : methods) {
        elements.append(method.get());
    }
    for (auto& constructor : constructors) {
        elements.append(constructor.get());
    }
    return elements;
}

QVector<const IClassElement*> Composition::get_new_code_elements() const {
    QVector<const IClassElement*> elements;
    for (auto& field : fields) {
        elements.append(field.get());
    }
    for (auto& method : methods) {
        elements.append(method.get());
    }
    for (auto& constructor : constructors) {
        elements.append(constructor.get());
    }
    if(copy_constructor_visibility.has_value()) {
        elements.append(get_copy_constructor().get());
    }

    // Potential leak?
    Destructor* destructor = new Destructor(get_destructor());
    elements.append(destructor);
    return elements;
}

QVector<const IClassElement*> Composition::get_code_elements() const {
    QVector<const IClassElement*> elements = get_new_code_elements();

    if (inheritance.has_value()) {
        auto inherited_elements = inheritance.value().second->get_code_elements();
        for (auto& elem : inherited_elements) {
            const Method* method = dynamic_cast<const Method*>(elem);
            if (method && method->get_method_kind() != MethodKind::Regular) {
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

    for (auto& elem : elements) {
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
            decl += "\t" + elem->declaration() + ";\n";
        }
    }

    decl += "};";
    return decl;
}

QString Composition::definition() const {
    QStringList definitions;
    auto elements = get_code_elements();

    for (auto& elem : elements) {
        auto def = elem->definition(name);
        if (def.has_value()) {
            definitions.append(def.value());
        }
    }

    return definitions.join("\n\n");
}

void Composition::add_field(const QString& name, const QString& type, std::optional<Visibility> visibility) {
    Visibility vis = visibility.value_or(get_default_visibility());

    fields.append(std::make_shared<Field>(name, type, vis));
}

void Composition::add_field(std::shared_ptr<Field> field){
    fields.append(field);
}

void Composition::add_method(const QString& name, const QString& type, Visibility visibility, MethodKind method_type, const QList<Argument*>& variables) {
    methods.append(std::make_shared<Method>(name, type, visibility, method_type, variables));
}

void Composition::add_method(std::shared_ptr<Method> method){
    methods.append(method);
}


void Composition::add_constructor(Visibility visibility, const QVector<Argument*>& arguments) {
    constructors.append(std::make_shared<DefaultConstructor>(name, arguments, visibility));
}

void Composition::add_copy_constructor(Visibility visibility) {
    copy_constructor_visibility = visibility;
}
void Composition::remove_copy_constructor() {
    copy_constructor_visibility = std::nullopt;
}

QString Composition::get_name() const {
    return name;
}

#include <model/elements/composition/cpp_class.h>
#include <model/elements/composition/cpp_struct.h>
#include <serializers/node_serializers.h>
#include <serializers/utils/utils.h>
#include <serializers/composition_element_serializers.h>

namespace {

    std::string serialize_visibility_value(const Visibility visibility){
        return get_declaration(visibility).toStdString();
    }

    Visibility deserialize_visibility_value(const std::string& visibility){
        if(visibility == "private"){
            return Visibility::Private;
        }else if(visibility == "protected"){
            return Visibility::Protected;
        }
        // visibility == "public"
        return Visibility::Public;
    }
};

void NodeSerializers::serialize_composition_node(std::ostream& output_stream, int indentation_counter, const Composition* composition_node)
{
    output_stream<< "{\n";
    ++indentation_counter;

    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter, "label", composition_node->get_label().toStdString());
    output_stream << ",\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter, "name", composition_node->get_name().toStdString());
    output_stream << ",\n";


    // [TODO] SERIALIZE copy constructor visibility


    // CONSTRUCTORS
    QList<DefaultConstructor*> constructors_raw_pointers = {};
    for (auto tmp : composition_node->constructors){
        constructors_raw_pointers.append(tmp.get());
    }

    SerializeHelpers::write_indented_serialized_list_field<DefaultConstructor>(output_stream, indentation_counter, "constructors", constructors_raw_pointers, &CompositionElementSerializers::serialize_constructor);

    // COPY CONSTRUCTOR
    bool has_copy_ctor = nullptr != composition_node->get_copy_constructor();
    if(has_copy_ctor){
        SerializeHelpers::write_indented_field(output_stream, indentation_counter, "copy_constructor", 1);
    }
    output_stream << ",\n";

    // FIELDS
    QList<Field*> fields_raw_pointers = {};
    for (auto tmp : composition_node->fields){
        fields_raw_pointers.append(tmp.get());
    }
    SerializeHelpers::write_indented_serialized_list_field<Field>(output_stream, indentation_counter, "fields", fields_raw_pointers, &CompositionElementSerializers::serialize_field);
    output_stream << ",\n";


    // METHODS
    QList<Method*> methods_raw_pointers = {};
    for (auto tmp : composition_node->methods){
        methods_raw_pointers.append(tmp.get());
    }
    SerializeHelpers::write_indented_serialized_list_field<Method>(output_stream, indentation_counter, "methods", methods_raw_pointers, &CompositionElementSerializers::serialize_method);
    output_stream << '\n';

    --indentation_counter;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "}";
}

std::shared_ptr<Composition> NodeSerializers::deserialize_composition_node(const QJsonObject json_composition){
    const auto& name = json_composition["name"].toString();
    const auto visibility = deserialize_visibility_value(json_composition["visibility"].toString().toStdString());

    Composition* composition_node = nullptr;
    const auto& label = json_composition["label"].toString();
    if(label == "class"){
        composition_node = new CPPClass(name, visibility);
    }else{ // label == "struct"
        composition_node = new CPPStruct(name, visibility);
    }

    const auto& json_fields = json_composition["fields"].toArray();
    for (const auto& json_field : json_fields){
        const auto field = CompositionElementSerializers::deserialize_field(json_field.toObject());
        composition_node->add_field(std::shared_ptr<Field>(field));
    }

    const auto& json_methods = json_composition["methods"].toArray();
    for (const auto& json_method : json_methods){
        const auto method = CompositionElementSerializers::deserialize_method(json_method.toObject());
        composition_node->add_method(std::shared_ptr<Method>(method));
    }

    if(json_composition.contains("copy_constructor")){
        composition_node->add_copy_constructor(Visibility::Public);
    }

    return std::shared_ptr<Composition>(composition_node);
}

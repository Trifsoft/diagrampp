#include <serializers/node_serializers.h>
#include <serializers/utils/utils.h>
#include <serializers/composition_element_serializers.h>


/*
 * Enum:
 * - label
 * - name
 * - enum values
 */

namespace {
    void serialize_enum_value(std::ostream& output_stream, int indentation_counter, const CPPEnumValue* enum_value){
        output_stream << "{\n";
        ++indentation_counter;
        SerializeHelpers::write_indented_string_field(output_stream, indentation_counter, "name", enum_value->get_name().toStdString());
        output_stream << ",\n";


        if(enum_value->get_value().has_value()){ //  value is optional
            SerializeHelpers::write_indented_field(output_stream, indentation_counter, "value", enum_value->get_value().value());
            output_stream << "\n";
        }
        --indentation_counter;
        SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "}";
    }

    std::string serialize_visibility_value(const Visibility visibility){
        switch(visibility){
        case Visibility::Private:
            return "private";
        case Visibility::Protected:
            return "protected";
        case Visibility::Public:
            return "public";
        }
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

void NodeSerializers::serialize_enum_node(std::ostream& output_stream, int indentation_counter, const CPPEnum* enum_node){
    output_stream<< "{\n";
    ++indentation_counter;

    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter, "label", "enum");
    output_stream << ",\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter, "name", enum_node->get_name().toStdString());
    output_stream << ",\n";

    SerializeHelpers::write_indented_serialized_list_field<CPPEnumValue>(output_stream, indentation_counter, "values", enum_node->get_values(), &serialize_enum_value);

    --indentation_counter;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "]\n";
    --indentation_counter;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "}";
}

std::shared_ptr<CPPEnum> NodeSerializers::deserialize_enum_node(QJsonObject json_enum) {
    QString name = json_enum["name"].toString();

    std::shared_ptr<CPPEnum> enum_node = std::make_shared<CPPEnum>(name);

    const QJsonArray& json_enum_values = json_enum["values"].toArray();
    for (const QJsonValue& json_value : json_enum_values){
        const QJsonObject& json_enum_value = json_value.toObject();
        const auto& name = json_enum_value["name"].toString();

        int value;
        if(json_enum_value.contains("value")){
            value = json_enum_value["value"].toInt();
            enum_node->add_value(name, value);
        }else {
            enum_node->add_value(name);
        }
    }
    return enum_node;
}



void NodeSerializers::serialize_composition_node(std::ostream& output_stream, int indentation_counter, const Composition* composition_node)
{
    output_stream<< "{\n";
    ++indentation_counter;

    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter, "label", composition_node->get_label().toStdString());
    output_stream << ",\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter, "name", composition_node->get_name().toStdString());
    output_stream << ",\n";


    // [TODO] SERIALIZE VISIBILITY & INHERITANCE
    // implement get methods for visiblity and inheritance in composition




    // CONSTRUCTORS
    QList<DefaultConstructor*> constructors_raw_pointers = {};
    for (auto tmp : composition_node->constructors){
        constructors_raw_pointers.append(tmp.get());
    }

    SerializeHelpers::write_indented_serialized_list_field<DefaultConstructor>(output_stream, indentation_counter, "constructors", constructors_raw_pointers, &CompositionElementSerializers::serialize_constructor);

    // DESTRUCTOR
    bool has_destructor = nullptr != composition_node->get_copy_constructor();
    SerializeHelpers::write_indented_field(output_stream, indentation_counter, "destructor", (has_destructor ? 1 : 0));
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
    for (auto tmp : composition_node->fields){
        fields_raw_pointers.append(tmp.get());
    }
    SerializeHelpers::write_indented_serialized_list_field<Method>(output_stream, indentation_counter, "method", methods_raw_pointers, &CompositionElementSerializers::serialize_method);
    output_stream << '\n';

    --indentation_counter;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "}";
}

std::shared_ptr<Composition> NodeSerializers::deserialize_composition_node(const QJsonObject json_composition){
    const auto& name = json_composition["name"].toString();
    const auto& label = json_composition["lable"].toString();

    const auto visibility = deserialize_visibility_value(json_composition["visibility"].toString().toStdString());

    // get inheritance
    // for object creation:
    // - name
    // - visibility
    // - inheritance


    // rest is through add methods


}

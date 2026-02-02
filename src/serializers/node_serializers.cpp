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

    const auto& json_enum_values = json_enum["values"].toArray();
    for (const auto& json_value : json_enum_values){
        const auto& json_enum_value = json_value.toObject();
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



void NodeSerializers::serialize_composition_node(std::ostream &output_stream, int indentation_counter, const Composition *composition_node)
{
    output_stream<< "{\n";
    ++indentation_counter;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "\"label\": ";  output_stream<< '\"' << composition_node->get_label().toStdString() << "\",\n" ;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "\"name\": "; output_stream<< composition_node->get_name().toStdString() << ",\n";

    // CONSTRUCTORS
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "\"constructors\": [\n";
    ++indentation_counter;

    auto& constructors = composition_node->constructors;
    for(auto constructor_it = constructors.begin(); constructor_it != constructors.end(); ){
        CompositionElementSerializers::serialize_constructor(output_stream, indentation_counter, constructor_it->get());
        if(++constructor_it == constructors.end()){
            output_stream<< '\n';
        }else{
            output_stream<< ",\n";
        }
    }

    --indentation_counter;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "], \n";


    // DESTRUCTOR
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "\"destructor\": {\n";
    ++indentation_counter;

    --indentation_counter;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "},\n";



    // FIELDS
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "\"fields\": [\n";
    ++indentation_counter;

    auto& fields = composition_node->fields;
    for (auto field_it = fields.begin(); field_it != fields.end(); ){
        CompositionElementSerializers::serialize_field(output_stream, indentation_counter, field_it->get());
        if(++field_it == fields.end()){
            output_stream<< '\n';
        }else{
            output_stream<< ",\n";
        }
    }

    --indentation_counter;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "], \n";

    // METHODS
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "\"methods\": [\n";
    ++indentation_counter;

    auto& methods = composition_node->methods;
    for (auto method_it = methods.begin(); method_it != methods.end(); ){
        CompositionElementSerializers::serialize_method(output_stream, indentation_counter, method_it->get());
        if(++method_it == methods.end()){
            output_stream<< '\n';
        }else{
            output_stream<< ",\n";
        }
    }
    --indentation_counter;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "]\n";


    --indentation_counter;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "}";

}

std::shared_ptr<Composition> NodeSerializers::deserialize_composition_node(const QJsonObject json_composition){

    ;

}

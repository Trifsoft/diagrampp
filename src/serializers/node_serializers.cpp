#include <serializers/node_serializers.h>
#include <serializers/utils/utils.h>
#include <serializers/composition_element_serializers.h>


/*
 * Enum:
 * - label
 * - name
 * - enum values
 */
void NodeSerializers::serialize_enum_node(std::ostream& output_stream, int indentation_counter, const CPPEnum* enum_node){
    output_stream<< "{\n";
    ++indentation_counter;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "\"label\": ";  output_stream<< "\"enum\",";
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "\"name\": "; output_stream<< enum_node->get_name().toStdString() << ",\n";
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "\"values\": [\n";
    ++indentation_counter;

    const auto& enum_values =  enum_node->get_values();
    for(auto enum_value = enum_values.begin(); enum_value != enum_values.end(); ){
        SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "{\n";
        ++indentation_counter;
        SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "\"name\": "; output_stream<< enum_value->get_name().toStdString() << ",\n";

        if(enum_value->get_value().has_value()){ // optional value
            SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "\"value\": "; output_stream<< enum_value->get_value().value() << "\n";
        }
        --indentation_counter;
        SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "}";

        if(++enum_value == enum_values.end()){
            output_stream<< '\n';
        }else {
            output_stream<< ",\n";
        }
    }

    --indentation_counter;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "]\n";
    --indentation_counter;
    SerializeHelpers::indent(output_stream, indentation_counter); output_stream<< "}";

}

CPPEnum* NodeSerializers::deserialize_enum_node(QJsonObject json_enum) {

    return nullptr;
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

    auto destructor = composition_node->get_destructor();
    CompositionElementSerializers::serialize_destructor(output_stream, indentation_counter, &destructor);

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

Composition* NodeSerializers::deserialize_composition_node(const QJsonObject json_composition)
{

    return nullptr;

}

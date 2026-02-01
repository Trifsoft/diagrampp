#include <serializers/composition_element_serializers.h>
#include <serializers/utils/utils.h>
#include <model/elements/type/regular_type.h>

void CompositionElementSerializers::serialize_method(std::ostream& output_stream, int indentation_counter, const Method *method)
{
    output_stream << "{\n";
    SerializeHelpers::write_indented_serialized_field<Description>(output_stream, indentation_counter+1, "description", method->get_description(), &CompositionElementSerializers::serialize_description);
    output_stream << ",\n";
    SerializeHelpers::write_indented_field<int>(output_stream, indentation_counter+1, "visibility", static_cast<int>(method->get_visibility()));
    output_stream << ",\n";
    SerializeHelpers::write_indented_field<int>(output_stream, indentation_counter+1, "method_type", static_cast<int>(method->get_method_type()));
    output_stream << ",\n";
    SerializeHelpers::write_indented_serialized_list_field<Description>(output_stream, indentation_counter+1, "variables", method->get_variables(), &CompositionElementSerializers::serialize_description);
    output_stream << ",\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter+1, "definition_block", method->get_definition_block().toStdString());
    output_stream << "\n";
    SerializeHelpers::indent(output_stream, indentation_counter);
    output_stream << "}";
}

Method *CompositionElementSerializers::deserialize_method(const QJsonObject json_method)
{
    Description* description = deserialize_description(json_method["description"].toObject());
    Visibility visibility = static_cast<Visibility>(json_method["visibility"].toInt());
    MethodType method_type = static_cast<MethodType>(json_method["method_type"].toInt());
    QList<Description*> variables;
    QJsonArray variables_json_array = json_method["variables"].toArray();
    for(auto element : variables_json_array) {
        variables.append(deserialize_description(element.toObject()));
    }
    QString definition_block = json_method["definition_block"].toString();
    return new Method(std::shared_ptr<Description>(description), visibility, method_type, variables, definition_block);
}

void CompositionElementSerializers::serialize_field(std::ostream& output_stream, int indentation_counter, const Field *field)
{
    output_stream << "{\n";
    SerializeHelpers::write_indented_serialized_field<Description>(output_stream, indentation_counter+1, "description", field->get_description(), &CompositionElementSerializers::serialize_description);
    output_stream << ",\n";
    SerializeHelpers::write_indented_field<int>(output_stream, indentation_counter+1, "visibility", static_cast<int>(field->get_visibility()));
    output_stream << ",\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter+1, "destruction", field->get_destruction().value_or("").toStdString());
    output_stream << "\n";
    SerializeHelpers::indent(output_stream, indentation_counter);
    output_stream << "}";
}

Field *CompositionElementSerializers::deserialize_field(const QJsonObject json_field)
{
    Description* description = CompositionElementSerializers::deserialize_description(json_field["description"].toObject());
    Visibility visibility = static_cast<Visibility>(json_field["visibility"].toInt());
    QString destruction = json_field["destruction"].toString();
    if(destruction.isEmpty()) {
        return new Field(std::shared_ptr<Description>(description), visibility);
    }
    else {
        return new Field(std::shared_ptr<Description>(description), visibility, destruction);
    }
}

void CompositionElementSerializers::serialize_constructor(std::ostream& output_stream, int indentation_counter, const Constructor *constructor)
{

}

Constructor *CompositionElementSerializers::deserialize_constructor(const QJsonObject json_constructor)
{

}

void CompositionElementSerializers::serialize_description(std::ostream& output_stream, int indentation_counter, const Description* description)
{
    output_stream << "{\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter+1, "name", description->get_name().toStdString());
    output_stream << ",\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter+1, "type", description->get_type()->get_name().toStdString()); //TODO izmeniti samo get_type().toStd kad se obrise IType
    output_stream << ",\n";
    SerializeHelpers::write_indented_field(output_stream, indentation_counter+1, "reference", static_cast<int>(description->get_reference()));
    output_stream << ",\n";
    SerializeHelpers::write_indented_field(output_stream, indentation_counter+1, "is_const", description->get_is_const());
    output_stream << "\n";
    SerializeHelpers::indent(output_stream, indentation_counter);
    output_stream << "}";
}

Description* CompositionElementSerializers::deserialize_description(const QJsonObject json_description)
{
    QString name = json_description["name"].toString();
    QString type = json_description["type"].toString();
    int reference = json_description["reference"].toInt();
    bool is_const = json_description["is_const"].toBool();
    return new Description(name, std::make_shared<RegularType>(type), static_cast<Reference>(reference), is_const); //TODO izmeniti type kad se obrise IType
}

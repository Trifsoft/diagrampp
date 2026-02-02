#include <serializers/composition_element_serializers.h>
#include <serializers/utils/utils.h>
#include <model/elements/type/regular_type.h>
#include <model/elements/constructor/copy_constructor.h>

void CompositionElementSerializers::serialize_method(std::ostream& output_stream, int indentation_counter, const Method *method)
{
    output_stream << "{\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter+1, "name", method->get_name().toStdString());
    output_stream << ",\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter+1, "type", method->get_return_type().toStdString());
    output_stream << ",\n";
    SerializeHelpers::write_indented_field<int>(output_stream, indentation_counter+1, "visibility", static_cast<int>(method->get_visibility()));
    output_stream << ",\n";
    SerializeHelpers::write_indented_field<int>(output_stream, indentation_counter+1, "method_kind", static_cast<int>(method->get_method_kind()));
    output_stream << ",\n";
    SerializeHelpers::write_indented_serialized_list_field<Argument>(output_stream, indentation_counter+1, "arguments", method->get_arguments(), &CompositionElementSerializers::serialize_argument);
    output_stream << ",\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter+1, "definition_block", method->get_definition_block().toStdString());
    output_stream << "\n";
    SerializeHelpers::indent(output_stream, indentation_counter);
    output_stream << "}";
}

Method *CompositionElementSerializers::deserialize_method(const QJsonObject json_method)
{
    QString name = json_method["name"].toString();
    QString type = json_method["type"].toString();
    Visibility visibility = static_cast<Visibility>(json_method["visibility"].toInt());
    MethodKind method_type = static_cast<MethodKind>(json_method["method_kind"].toInt());
    QList<Argument*> arguments;
    QJsonArray variables_json_array = json_method["arguments"].toArray();
    for(auto element : variables_json_array) {
        arguments.append(deserialize_argument(element.toObject()));
    }
    QString definition_block = json_method["definition_block"].toString();
    return new Method(name, type, visibility, method_type, arguments, definition_block);
}

void CompositionElementSerializers::serialize_field(std::ostream& output_stream, int indentation_counter, const Field *field)
{
    output_stream << "{\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter+1, "name", field->get_name().toStdString());
    output_stream << ",\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter+1, "type", field->get_type().toStdString());
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
    QString name = json_field["name"].toString();
    QString type = json_field["type"].toString();
    Visibility visibility = static_cast<Visibility>(json_field["visibility"].toInt());
    QString destruction = json_field["destruction"].toString();
    if(destruction.isEmpty()) {
        return new Field(name, type, visibility);
    }
    else {
        return new Field(name, type, visibility, destruction);
    }
}

void CompositionElementSerializers::serialize_constructor(std::ostream& output_stream, int indentation_counter, const DefaultConstructor *constructor)
{
    output_stream << "{\n";
    SerializeHelpers::write_indented_field<int>(output_stream, indentation_counter+1, "visibility", static_cast<int>(constructor->get_visibility()));
    output_stream << ",\n";
    SerializeHelpers::write_indented_serialized_list_field<Argument>(output_stream, indentation_counter+1, "arguments", constructor->get_arguments(), &CompositionElementSerializers::serialize_argument);
    output_stream << "\n";
    SerializeHelpers::indent(output_stream, indentation_counter);
    output_stream << "}";
}
DefaultConstructor *CompositionElementSerializers::deserialize_constructor(const QJsonObject json_constructor, const QString& class_name)
{
    Visibility visibility = static_cast<Visibility>(json_constructor["visibility"].toInt());
    QList<Argument*> arguments;
    QJsonArray arguments_json_array = json_constructor["arguments"].toArray();
    for(auto element : arguments_json_array) {
        arguments.append(deserialize_argument(element.toObject()));
    }
    return new DefaultConstructor(class_name, arguments, visibility);
}

void CompositionElementSerializers::serialize_argument(std::ostream& output_stream, int indentation_counter, const Argument* description)
{
    output_stream << "{\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter+1, "name", description->get_name().toStdString());
    output_stream << ",\n";
    SerializeHelpers::write_indented_string_field(output_stream, indentation_counter+1, "type", description->get_type().toStdString());
    output_stream << "\n";
    SerializeHelpers::indent(output_stream, indentation_counter);
    output_stream << "}";
}

Argument* CompositionElementSerializers::deserialize_argument(const QJsonObject json_description)
{
    QString name = json_description["name"].toString();
    QString type = json_description["type"].toString();
    return new Argument(name, type);
}

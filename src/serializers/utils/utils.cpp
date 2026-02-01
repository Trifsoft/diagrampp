#include <serializers/utils/utils.h>

void SerializeHelpers::indent(std::ostream& output_stream, const int indentation_counter){
    for (int i = 0; i < indentation_counter; i++){
        output_stream << "\t";
    }
}

void write_with_quotes(std::ostream& output_stream, const std::string& text) {
    output_stream << '"' << text << "\": ";
}

void SerializeHelpers::write_indented_string_field(std::ostream& output_stream, const int indentation_counter, const std::string& key, const std::string& value) {
    SerializeHelpers::indent(output_stream, indentation_counter);
    write_with_quotes(output_stream, key);
    output_stream << ": ";
    write_with_quotes(output_stream, value);
}

template <typename T>
void SerializeHelpers::write_indented_field(std::ostream& output_stream, const int indentation_counter, const std::string& key, const T& value) {
    SerializeHelpers::indent(output_stream, indentation_counter);
    write_with_quotes(output_stream, key);
    output_stream << ": " << value;
}

template <typename T>
void SerializeHelpers::write_indented_serialized_field(std::ostream& output_stream, const int indentation_counter, const std::string& key, T* value, std::function<void(std::ostream&, const int, T*)> serializer) {
    SerializeHelpers::indent(output_stream, indentation_counter);
    write_with_quotes(output_stream, key);
    output_stream << ": ";

    serializer(output_stream, indentation_counter+1, value);

}

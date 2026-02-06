#include <serializers/utils/utils.h>

void SerializeHelpers::indent(std::ostream& output_stream, const int indentation_counter){
    for (int i = 0; i < indentation_counter; i++){
        output_stream << "\t";
    }
}

void SerializeHelpers::write_with_quotes(std::ostream& output_stream, const std::string& text) {
    output_stream << '"' << text << "\" ";
}

void SerializeHelpers::write_indented_string_field(std::ostream& output_stream, const int indentation_counter, const std::string& key, const std::string& value) {
    SerializeHelpers::indent(output_stream, indentation_counter);
    write_with_quotes(output_stream, key);
    output_stream << ": ";
    write_with_quotes(output_stream, value);
}

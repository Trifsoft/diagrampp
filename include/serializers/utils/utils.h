#ifndef UTILS_H
#define UTILS_H

#include <ostream>
#include <functional>

namespace SerializeHelpers{
    void indent(std::ostream& output_stream, const int indentation_counter);

    void write_indented_string_field(std::ostream& output_stream, const int indentation_counter, const std::string& key, const std::string& value);
    void write_with_quotes(std::ostream& output_stream, const std::string& key);

    template <typename T>
    void write_indented_field(std::ostream& output_stream, const int indentation_counter, const std::string& key, const T& value) {
        SerializeHelpers::indent(output_stream, indentation_counter);
        write_with_quotes(output_stream, key);
        output_stream << ": " << value;
    }

    template <typename T>
    void write_indented_serialized_field(std::ostream& output_stream, const int indentation_counter, const std::string& key, T* value, std::function<void(std::ostream&, const int, T*)> serializer) {
        SerializeHelpers::indent(output_stream, indentation_counter);
        write_with_quotes(output_stream, key);
        output_stream << ": ";

        serializer(output_stream, indentation_counter+1, value);

    }
}

#endif // UTILS_H

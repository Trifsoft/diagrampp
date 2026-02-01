#ifndef UTILS_H
#define UTILS_H

#include <ostream>
#include <functional>

namespace SerializeHelpers{
    void indent(std::ostream& output_stream, const int indentation_counter);

    void write_indented_string_field(std::ostream& output_stream, const int indentation_counter, const std::string& key, const std::string& value);

    template <typename T>
    void write_indented_field(std::ostream& output_stream, const int indentation_counter, const std::string& key, const T& value);

    template <typename T>
    void write_indented_serialized_field(std::ostream& output_stream, const int indentation_counter, const std::string& key, T* value, std::function<void(std::ostream&, const int, T*)> serializer);
}

#endif // UTILS_H

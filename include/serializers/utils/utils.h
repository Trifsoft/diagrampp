#ifndef UTILS_H
#define UTILS_H

#include <ostream>
#include <functional>
#include <QList>

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

    template <typename T>
    void write_indented_serialized_list_field(std::ostream& output_stream, const int indentation_counter, const std::string& key, const QList<T*>& list, std::function<void(std::ostream&, const int, T*)> serializer) {
        SerializeHelpers::indent(output_stream, indentation_counter);
        write_with_quotes(output_stream, key);
        output_stream << ": [\n";
        for(auto element_it = list.begin(); element_it != list.end();) {
            SerializeHelpers::indent(output_stream, indentation_counter+1);
            serializer(output_stream, indentation_counter+1, (*element_it));

            if(++element_it == list.end()){
                output_stream << '\n';
            }
            else {
                output_stream << ",\n";
            }
        }
        SerializeHelpers::indent(output_stream, indentation_counter);
        output_stream << "]";
    }
}

#endif // UTILS_H

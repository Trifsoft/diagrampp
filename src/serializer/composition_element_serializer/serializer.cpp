#include <serializer/composition_element_serializer/serializer.h>

CompositionElementSerializer::CompositionElementSerializer(std::fstream* file_stream, const int indent_counter) {
    m_file_stream = file_stream;
    m_indentation_counter = indent_counter;
}

void CompositionElementSerializer::indent(){
    for (int i = 0; i < m_indentation_counter; i++){
        *m_file_stream << "\t";
    }
}

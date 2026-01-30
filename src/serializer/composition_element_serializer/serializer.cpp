#include <serializer/composition_element_serializer/serializer.h>

CompositionElementSerializer::CompositionElementSerializer(std::stringstream* writing_stream, std::stringstream* reading_stream, const int indent_counter){
    m_output_stream = writing_stream;
    m_input_stream = reading_stream;
    m_indentation_counter = indent_counter;
}

void CompositionElementSerializer::indent(){
    for (int i = 0; i < m_indentation_counter; i++){
        *m_output_stream << "\t";
    }
}

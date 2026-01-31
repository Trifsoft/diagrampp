#include <serializer/node_serializer/serializer.h>

NodeSerializer::NodeSerializer(std::fstream* file_stream, const int indent_counter){
    m_file_stream = file_stream;
    m_indentation_counter = indent_counter;
}


void NodeSerializer::indent(){
    for (int i = 0; i < m_indentation_counter; i++){
        *m_file_stream << "\t";
    }
}

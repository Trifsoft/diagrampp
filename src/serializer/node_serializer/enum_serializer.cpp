#include "serializer/node_serializer/enum_serializer.h"

/*
 * Enum:
 * - label
 * - name
 * - enum values
 */


EnumSerializer::EnumSerializer(std::fstream* file_stream, const int indentation_counter, CPPEnum* en)
    : NodeSerializer(file_stream, indentation_counter)
{
    m_enum_node = en;
}


void EnumSerializer::serialize(){
    *m_file_stream << "{\n";
    ++m_indentation_counter;
    indent(); *m_file_stream << "\"label\": ";  *m_file_stream << "\"enum\",";
    indent(); *m_file_stream << "\"name\": "; *m_file_stream << m_enum_node->get_name().toStdString() << ",\n";
    indent(); *m_file_stream << "\"values\": [\n";
    ++m_indentation_counter;

    const auto& enum_values =  m_enum_node->get_values();
    for(auto enum_value = enum_values.begin(); enum_value != enum_values.end(); ){
        indent(); *m_file_stream << "{\n";
        ++m_indentation_counter;
        indent(); *m_file_stream << "\"name\": "; *m_file_stream << enum_value->get_name().toStdString() << ",\n";

        if(enum_value->get_value().has_value()){ // optional value
            indent(); *m_file_stream << "\"value\": "; *m_file_stream << enum_value->get_value().value() << "\n";
        }
        --m_indentation_counter;
        indent(); *m_file_stream << "}";

        if(++enum_value == enum_values.end()){
            *m_file_stream << '\n';
        }else {
            *m_file_stream << ",\n";
        }
    }

    --m_indentation_counter;
    indent(); *m_file_stream << "]\n";
    --m_indentation_counter;
    indent(); *m_file_stream << "}";

}

IUMLClassDiagramNode* EnumSerializer::deserialize() {

}

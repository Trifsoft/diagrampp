#include "serializer/node_serializer/enum_serializer.h"

/*
 * Enum:
 * - label
 * - name
 * - enum values
 */


EnumSerializer::EnumSerializer(CPPEnum* en, std::stringstream* writing_stream, std::stringstream* reading_stream, const int indentation_counter)
    : NodeSerializer(writing_stream, reading_stream, indentation_counter)
{
    if(!writing_stream){ // deserialization
        m_enum_node = nullptr; // intialized in deserialization method
    }else{
        m_enum_node = en;
    }
}


std::string EnumSerializer::serialize(){
    *m_output_stream << "{\n";
    ++m_indentation_counter;
    indent(); *m_output_stream << "\"label\": ";  *m_output_stream << "\"enum\",";
    indent(); *m_output_stream << "\"name\": "; *m_output_stream << m_enum_node->get_name().toStdString() << ",\n";
    indent(); *m_output_stream << "\"values\": [\n";
    ++m_indentation_counter;

    const auto& enum_values =  m_enum_node->get_values();
    for(auto enum_value = enum_values.begin(); enum_value != enum_values.end(); ){
        indent(); *m_output_stream << "{\n";
        ++m_indentation_counter;
        indent(); *m_output_stream << "\"name\": "; *m_output_stream << enum_value->get_name().toStdString() << ",\n";

        if(enum_value->get_value().has_value()){ // optional value
            indent(); *m_output_stream << "\"value\": "; *m_output_stream << enum_value->get_value().value() << "\n";
        }
        --m_indentation_counter;
        indent(); *m_output_stream << "}";

        if(++enum_value == enum_values.end()){
            *m_output_stream << '\n';
        }else {
            *m_output_stream << ",\n";
        }
    }

    --m_indentation_counter;
    indent(); *m_output_stream << "]\n";
    --m_indentation_counter;
    indent(); *m_output_stream << "}";

    return (*m_output_stream).str();
}

IUMLClassDiagramNode* EnumSerializer::deserialize() {

}

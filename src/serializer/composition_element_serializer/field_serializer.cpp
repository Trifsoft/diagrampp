#include <serializer/composition_element_serializer/field_serializer.h>

FieldSerializer::FieldSerializer(std::fstream* file_stream, const int indentation_counter, Field* f)
    : CompositionElementSerializer(file_stream, indentation_counter)
{
    m_field = f;
}

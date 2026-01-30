#include <serializer/composition_element_serializer/field_serializer.h>

FieldSerializer::FieldSerializer(Field* f, std::stringstream* writing_stream, std::stringstream* reading_stream, const int indentation_counter)
    : CompositionElementSerializer(writing_stream, reading_stream, indentation_counter)
{
    if(!writing_stream){ // deserialization
        m_field = nullptr; // intialized in deserialization method
    }else{
        m_field = f;
    }
}

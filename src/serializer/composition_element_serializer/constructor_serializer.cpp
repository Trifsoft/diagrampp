#include <serializer/composition_element_serializer/constructor_serializer.h>


ConstructorSerializer::ConstructorSerializer(std::fstream* file_stream, const int indentation_counter, Constructor* c)
    : CompositionElementSerializer(file_stream, indentation_counter)
{
    m_constructor = c;
}

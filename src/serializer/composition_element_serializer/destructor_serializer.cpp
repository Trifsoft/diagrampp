#include <serializer/composition_element_serializer/destructor_serializer.h>

DestructorSerializer::DestructorSerializer(std::fstream* file_stream, const int indentation_counter, Destructor* d)
    : CompositionElementSerializer(file_stream, indentation_counter)
{
    m_destructor = d;
}

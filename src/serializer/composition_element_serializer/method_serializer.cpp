#include <serializer/composition_element_serializer/method_serializer.h>

MethodSerializer::MethodSerializer(std::fstream* file_stream, const int indentation_counter, Method* m)
    : CompositionElementSerializer(file_stream, indentation_counter)
{
    m_method = m;
}

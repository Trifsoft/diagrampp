#include <serializer/composition_element_serializer/method_serializer.h>

MethodSerializer::MethodSerializer(Method* m, std::stringstream* writing_stream, std::stringstream* reading_stream, const int indentation_counter)
    : CompositionElementSerializer(writing_stream, reading_stream, indentation_counter)
{
    if(!writing_stream){ // deserialization
        m_method = nullptr; // intialized in deserialization method
    }else{
        m_method = m;
    }
}

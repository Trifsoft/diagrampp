#include <serializer/composition_element_serializer/constructor_serializer.h>


ConstructorSerializer::ConstructorSerializer(Constructor* c, std::stringstream* writing_stream, std::stringstream* reading_stream, const int indentation_counter)
    : CompositionElementSerializer(writing_stream, reading_stream, indentation_counter)
{
    if(!writing_stream){ // deserialization
        m_constructor = nullptr; // intialized in deserialization method
    }else{
        m_constructor = c;
    }
}

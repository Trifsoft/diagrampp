#include <serializer/composition_element_serializer/destructor_serializer.h>

DestructorSerializer::DestructorSerializer(Destructor& d, std::stringstream* writing_stream, std::stringstream* reading_stream, const int indentation_counter)
    : CompositionElementSerializer(writing_stream, reading_stream, indentation_counter)
{
    if(!writing_stream){ // deserialization
        m_destructor = nullptr; // intialized in deserialization method
    }else{
        m_destructor = &d;
    }
}

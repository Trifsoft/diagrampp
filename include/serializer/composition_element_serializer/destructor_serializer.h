#ifndef DESTRUCTOR_SERIALIZER_H
#define DESTRUCTOR_SERIALIZER_H

#include <serializer/composition_element_serializer/serializer.h>
#include <model/elements/destructor.h>

class DestructorSerializer : CompositionElementSerializer{
private:
    Destructor* m_destructor;
public:
    DestructorSerializer(std::fstream* file_stream, const int indentation_counter, Destructor* d = nullptr);

    void serialize() override; // should print just properties, without {}
    Destructor* deserialize() override;
};

#endif // DESTRUCTOR_SERIALIZER_H

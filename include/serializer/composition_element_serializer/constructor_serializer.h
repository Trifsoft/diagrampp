#ifndef CONSTRUCTOR_SERIALIZER_H
#define CONSTRUCTOR_SERIALIZER_H

#include <serializer/composition_element_serializer/serializer.h>
#include <model/elements/constructor/constructor.h>

class ConstructorSerializer : CompositionElementSerializer{
private:
    Constructor* m_constructor;
public:
    ConstructorSerializer(std::fstream* file_stream, const int indentation_counter, Constructor* c = nullptr);

    void serialize() override;
    Constructor* deserialize() override;
};


#endif // CONSTRUCTOR_SERIALIZER_H

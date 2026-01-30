#ifndef CONSTRUCTOR_SERIALIZER_H
#define CONSTRUCTOR_SERIALIZER_H

#include <serializer/composition_element_serializer/serializer.h>
#include <model/elements/constructor/constructor.h>

class ConstructorSerializer : CompositionElementSerializer{
private:
    Constructor* m_constructor;
public:
    ConstructorSerializer(Constructor* c, std::stringstream* writing_stream, std::stringstream* reading_stream, const int indentation_counter);

    std::string serialize() override;
    Constructor* deserialize() override;
};


#endif // CONSTRUCTOR_SERIALIZER_H

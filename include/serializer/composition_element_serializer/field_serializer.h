#ifndef FIELD_SERIALIZER_H
#define FIELD_SERIALIZER_H

#include <model/elements/field.h>
#include <serializer/composition_element_serializer/serializer.h>

class FieldSerializer : CompositionElementSerializer{
private:
    Field* m_field;
public:
    FieldSerializer(Field* f, std::stringstream* writing_stream, std::stringstream* reading_stream, const int indentation_counter);

    std::string serialize() override;
    Field* deserialize() override;
};

#endif // FIELD_SERIALIZER_H

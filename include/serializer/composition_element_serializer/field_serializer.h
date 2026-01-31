#ifndef FIELD_SERIALIZER_H
#define FIELD_SERIALIZER_H

#include <model/elements/field.h>
#include <serializer/composition_element_serializer/serializer.h>
#include <fstream>

class FieldSerializer : CompositionElementSerializer{
private:
    Field* m_field;
public:
    FieldSerializer(std::fstream* file_stream, const int indentation_counter, Field* f = nullptr);

    void serialize() override;
    Field* deserialize() override;
};

#endif // FIELD_SERIALIZER_H

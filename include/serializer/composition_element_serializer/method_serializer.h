#ifndef METHOD_SERIALIZER_H
#define METHOD_SERIALIZER_H

#include <serializer/composition_element_serializer/serializer.h>
#include <model/elements/method.h>

class MethodSerializer : CompositionElementSerializer{
private:
    Method* m_method;
public:
    MethodSerializer(Method* m, std::stringstream* writing_stream, std::stringstream* reading_stream, const int indentation_counter);

    std::string serialize() override;
    Method* deserialize() override;
};

#endif // METHOD_SERIALIZER_H

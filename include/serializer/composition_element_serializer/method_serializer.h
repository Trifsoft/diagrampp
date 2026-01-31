#ifndef METHOD_SERIALIZER_H
#define METHOD_SERIALIZER_H

#include <serializer/composition_element_serializer/serializer.h>
#include <model/elements/method.h>
#include <fstream>

class MethodSerializer : CompositionElementSerializer{
private:
    Method* m_method;
public:
    MethodSerializer(std::fstream* file_stream, const int indentation_counter, Method* m = nullptr);

    void serialize() override;
    Method* deserialize() override;
};

#endif // METHOD_SERIALIZER_H

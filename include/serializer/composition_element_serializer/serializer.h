#ifndef CLASS_ELEMENT_SERIALIZER_H
#define CLASS_ELEMENT_SERIALIZER_H

#include <sstream>
#include <model/base/class_element.h>

class CompositionElementSerializer
{
protected:
    int m_indentation_counter = 0;
    std::stringstream* m_output_stream = nullptr;
    std::stringstream* m_input_stream = nullptr;

    void indent();
public:
    CompositionElementSerializer(std::stringstream* writing_stream, std::stringstream* reading_stream, const int indentation_counter);
    virtual std::string serialize() = 0;
    virtual IClassElement* deserialize() = 0;
};

#endif // CLASS_ELEMENT_SERIALIZER_H

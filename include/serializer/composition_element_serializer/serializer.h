#ifndef CLASS_ELEMENT_SERIALIZER_H
#define CLASS_ELEMENT_SERIALIZER_H

#include <fstream>
#include <model/base/class_element.h>

class CompositionElementSerializer
{
protected:
    int m_indentation_counter = 0;
    std::fstream* m_file_stream;

    void indent();
public:
    CompositionElementSerializer(std::fstream* file_stream, const int indentation_counter);
    virtual void serialize() = 0;
    virtual IClassElement* deserialize() = 0;
};

#endif // CLASS_ELEMENT_SERIALIZER_H

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <sstream>
#include <model/base/uml_class_diagram_node.h>

class NodeSerializer
{
protected:
    int m_indentation_counter = 0;
    std::stringstream* m_output_stream = nullptr;
    std::stringstream* m_input_stream = nullptr;

    void indent();
public:
    NodeSerializer(std::stringstream* writing_stream, std::stringstream* reading_stream, const int indentation_counter);
    virtual std::string serialize() = 0;
    virtual IUMLClassDiagramNode* deserialize() = 0;
};

#endif // SERIALIZER_H

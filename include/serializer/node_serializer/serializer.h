#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <fstream>
#include <model/base/uml_class_diagram_node.h>

class NodeSerializer
{
protected:
    int m_indentation_counter = 0;
    std::fstream* m_file_stream = nullptr;

    void indent();
public:
    NodeSerializer(std::fstream* file_stream, const int indentation_counter);
    virtual void serialize() = 0;
    virtual IUMLClassDiagramNode* deserialize() = 0;
};

#endif // SERIALIZER_H

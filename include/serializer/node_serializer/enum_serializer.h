#ifndef ENUMPARSER_H
#define ENUMPARSER_H

#include <model/elements/composition/cpp_enum.h>
#include <serializer/node_serializer/serializer.h>

class EnumSerializer : NodeSerializer{
private:
    CPPEnum* m_enum_node;
public:
    EnumSerializer(CPPEnum* ce, std::stringstream* writing_stream, std::stringstream* reading_stream, const int indentation_counter);

    std::string serialize() override;
    IUMLClassDiagramNode* deserialize() override;
};

#endif // ENUMPARSER_H

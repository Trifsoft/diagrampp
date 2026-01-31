#ifndef CLASS_SERIALIZER_H
#define CLASS_SERIALIZER_H

#include <model/base/uml_class_diagram_node.h>
#include <serializer/node_serializer/serializer.h>
#include <model/elements/constructor/constructor.h>
#include <model/elements/destructor.h>
#include <model/elements/field.h>
#include <model/elements/method.h>
#include <model/elements/composition/composition.h>

class CompositionSerializer : NodeSerializer{
private:
    Composition* m_composition_node;
public:
    CompositionSerializer(std::fstream* file_stream, const int indentation_counter, Composition* cs = nullptr);
    void serialize() override;
    Composition* deserialize() override;
};

#endif // CLASS_SERIALIZER_H

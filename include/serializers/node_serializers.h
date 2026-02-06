#ifndef NODE_SERIALIZERS_H
#define NODE_SERIALIZERS_H

#include <model/elements/composition/composition.h>
#include <model/base/uml_class_diagram_node.h>
#include <QJsonObject>

namespace NodeSerializers
{
    void serialize_composition_node(std::ostream& output_stream, int indentation_counter, const Composition* composition_node);
    std::shared_ptr<Composition> deserialize_composition_node(const QJsonObject json_composition);
};

#endif // NODE_SERIALIZERS_H

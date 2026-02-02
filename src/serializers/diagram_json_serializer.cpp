#include <serializers/diagram_json_serializer.h>
#include <model/base/uml_class_diagram_node.h>
#include <serializers/node_serializers.h>
#include <serializers/utils/utils.h>

/*
 * rules:
 * 1) every function is obligated to respect indentation, and print its content inside {} or []
 * 2) no function print new line at the end at passed output_stream, last char should be } or ]
 * 3) DiagramJsonSerializer json is reponsible for printing new lines, commas, and brackets between whole sections(one object or array in json)
 *
 */

/* JSON structure
 * [
 *      {
 *          coords:
 *          node:
 *          neighbours: [
 *                {
 *                  coords:
 *                  node:
 *                  branch_type:
 *                }
 *          ]
 *      }
 * ]
 *
 */

DiagramJsonSerializer::DiagramJsonSerializer(graph_type* m_diagram_graph) {
    m_diagram = m_diagram_graph;
    m_indentation_counter = 0;
}

void DiagramJsonSerializer::call_corresponding_node_serializer(std::ostream& output_stream, IUMLClassDiagramNode* node) const {
    auto label = node->get_label().toStdString();

    if(label == "class" || label == "struct"){
        Composition* c = dynamic_cast<Composition*>(node);
        NodeSerializers::serialize_composition_node(output_stream, m_indentation_counter, c);
    }else if(label == "enum") {
        CPPEnum* e = dynamic_cast<CPPEnum*>(node);
        NodeSerializers::serialize_enum_node(output_stream, m_indentation_counter, e);
    }
}

void DiagramJsonSerializer::trim_end(std::string& s) const {
    s.erase(
        std::find_if(s.rbegin(), s.rend(),[](unsigned char ch) { return !std::isspace(ch); }).base(),
        s.end()
    );
}

void DiagramJsonSerializer::write_coords(std::ostream& output_stream, double x, double y){
    SerializeHelpers::indent(output_stream, m_indentation_counter); output_stream << "\"coords\": {\n";
    ++m_indentation_counter;

    SerializeHelpers::indent(output_stream, m_indentation_counter); output_stream << "\"x\": "; output_stream << x << ',' << '\n';

    SerializeHelpers::indent(output_stream, m_indentation_counter); output_stream << "\"y\": "; output_stream << y << '\n';

    --m_indentation_counter;
    SerializeHelpers::indent(output_stream, m_indentation_counter); output_stream << '}';
}

void DiagramJsonSerializer::write_branch_type(std::ostream& output_stream, BranchType branch_type) const {
    SerializeHelpers::indent(output_stream, m_indentation_counter);
    switch(branch_type){
        case BranchType::INHERITANCE:
            output_stream << "\"branch_type\": " << "\"inheritance\"";
            break;
        case BranchType::ASSOCIATION:
            output_stream << "\"branch_type\": " << "\"association\"";
            break;
        case BranchType::REALIZATION:
            output_stream << "\"branch_type\": " << "\"realization\"";
            break;
        case BranchType::AGGREGATION:
            output_stream << "\"branch_type\": " << "\"aggregation\"";
            break;
        case BranchType::COMPOSITION:
            output_stream << "\"branch_type\": " << "\"composition\"";
            break;
        case BranchType::DEPENDENCY:
            output_stream << "\"branch_type\": " << "\"dependency\"";
            break;
    }
}

void DiagramJsonSerializer::serialize(std::ostream& output_stream){
    int m_indentation_counter = 0;
    output_stream << "[\n";
    ++m_indentation_counter;
    SerializeHelpers::indent(output_stream, m_indentation_counter);

    for (auto it = m_diagram->begin(); it != m_diagram->end(); ){
        auto& node_view = it->first;
        output_stream << "{\n";
        ++m_indentation_counter;

        // 1) coords
        write_coords(output_stream, node_view->x(), node_view->y());
        output_stream << ",\n";

        // 2) node
        const auto node = node_view->get_uml_class_diagram_node(); // does not return const IUMLClassDiagramNode
        call_corresponding_node_serializer(output_stream, node);
        output_stream << ",\n";

        // 3) neighbours (list of <{node, coords}, branch_type}>)
        output_stream << "\"neighbours\":" << '[' << '\n';
        ++m_indentation_counter;
        for (auto neighbour_it = it->second.begin(); neighbour_it != it->second.end(); ){
            SerializeHelpers::indent(output_stream, m_indentation_counter); output_stream << "{\n";
            ++m_indentation_counter;

            SerializeHelpers::indent(output_stream, m_indentation_counter); write_branch_type(output_stream, neighbour_it->second); output_stream << ",\n";


            const auto& neighbour = neighbour_it->first->get_uml_class_diagram_node();
            call_corresponding_node_serializer(output_stream, neighbour); output_stream << ",\n";

            SerializeHelpers::indent(output_stream, m_indentation_counter); write_coords(output_stream, neighbour_it->first->x(), neighbour_it->first->y()); output_stream << '\n';
            --m_indentation_counter;
            SerializeHelpers::indent(output_stream, m_indentation_counter); output_stream << "}";

            if(++neighbour_it == it->second.end()){
                output_stream << '\n';
            }else {
                output_stream << ",\n";
            }
        }

        --m_indentation_counter;
        SerializeHelpers::indent(output_stream, m_indentation_counter); output_stream << "]\n";

        --m_indentation_counter;
        SerializeHelpers::indent(output_stream, m_indentation_counter); output_stream << "}";

        if(++it == m_diagram->end()){
            output_stream << '\n';
        }else{
            output_stream << ",\n";
        }
    }

    --m_indentation_counter;
    SerializeHelpers::indent(output_stream, m_indentation_counter);
    output_stream << "]\n";

}

std::pair<double, double> DiagramJsonSerializer::deserialize_coords(QJsonObject json_coords) const {
    return {json_coords["x"].toDouble(), json_coords["y"].toDouble()};
}

std::shared_ptr<IUMLClassDiagramNode> DiagramJsonSerializer::deserialize_node(QJsonObject json_node) const {
    if(json_node["label"] == "class" || json_node["label"] == "struct"){
        return NodeSerializers::deserialize_composition_node(json_node);
    }else{ // label = enum
        return NodeSerializers::deserialize_enum_node(json_node);
    }
}

BranchType DiagramJsonSerializer::deserialize_branch_type(QJsonValue json_branch_type) const {
    std::string branch_type = json_branch_type.toString().toStdString();
    if(branch_type == "inheritance"){
        return BranchType::INHERITANCE;
    }else if(branch_type == "association"){
        return BranchType::ASSOCIATION;
    }else if(branch_type == "realization"){
        return BranchType::REALIZATION;
    }else if(branch_type == "aggregation"){
        return BranchType::AGGREGATION;
    }else if(branch_type == "composition"){
        return BranchType::COMPOSITION;
    }

    // branch_type == dependency
    return BranchType::DEPENDENCY;
}

graph_type* DiagramJsonSerializer::deserialize(QJsonArray json_array){

    for(const auto& json_value : json_array){
        QJsonObject json_object = json_value.toObject();
        const auto [x,y] = deserialize_coords(json_object["coords"].toObject());
        // diagram_coord_map, write them in coord map or something
        // coords should be passed to board somehow, see comment down bellow


        std::shared_ptr<IUMLClassDiagramNode> node = deserialize_node(json_object["node"].toObject());
        // waiting for model view architecture border line

        QJsonArray neighbours = json_object["neighbours"].toArray();
        for (const auto& json_neighbour_val : neighbours){
            const auto& json_neighbour_object = json_neighbour_val.toObject();

            const auto [neighbour_x, neighbour_y] = deserialize_coords(json_object["coords"].toObject());
            const auto node = deserialize_node(json_neighbour_object["node"].toObject());
            const auto branch_type = deserialize_branch_type(json_object["branch_type"].toObject());

            // add this to diagram
        }
    }

    return m_diagram;
}

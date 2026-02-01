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

void DiagramJsonSerializer::write_inheritance(std::ostream& output_stream, BranchType branch_type) const {
    SerializeHelpers::indent(output_stream, m_indentation_counter);
    switch(branch_type){
        case BranchType::INHERITANCE:
            output_stream << "\"branch_type\": " << "\"inheritance\"";
            break;
        case BranchType::ASSOCIATION:
            output_stream << "\"branch_type\": " << "\"association\"";
            break;
        case BranchType::NAVIGATION:
            output_stream << "\"branch_type\": " << "\"navigation\"";
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
    ;
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

            SerializeHelpers::indent(output_stream, m_indentation_counter); write_inheritance(output_stream, neighbour_it->second); output_stream << ",\n";


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

graph_type* DiagramJsonSerializer::deserialize(QJsonArray json_array){

    return nullptr;
}

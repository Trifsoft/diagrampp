#include <serializers/diagram_json_serializer.h>
#include <model/base/uml_class_diagram_node.h>
#include <serializers/node_serializers.h>
#include <serializers/utils/utils.h>
/*
 * rules:
 * 1) every serialize function is obligated to respect indentation, and print its content inside {} or []
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


DiagramJsonSerializer::DiagramJsonSerializer(Board* board)
    : m_board(board), m_indentation_counter(0)
{

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

    SerializeHelpers::write_indented_field(output_stream, m_indentation_counter, "x", x);
    output_stream << ",\n";
    SerializeHelpers::write_indented_field(output_stream, m_indentation_counter, "y", y);
    output_stream << '\n';

    --m_indentation_counter;
    SerializeHelpers::indent(output_stream, m_indentation_counter); output_stream << '}';
}

void DiagramJsonSerializer::write_branch_type(std::ostream& output_stream, BranchType branch_type) const {
    const std::string& branch_string = get_string_from_branch_type(branch_type);
    SerializeHelpers::write_indented_string_field(output_stream, m_indentation_counter, "branch_type", branch_string);
}

void DiagramJsonSerializer::serialize_neighbours(std::ostream& output_stream, const std::vector<std::pair<SharedNodePtr, BranchType>>& neighbours){
    SerializeHelpers::indent(output_stream, m_indentation_counter);
    SerializeHelpers::write_with_quotes(output_stream, "neighbours");
    output_stream << ": ";
    output_stream << "[\n";
    ++m_indentation_counter;

    for (auto neighbour_it = neighbours.begin(); neighbour_it != neighbours.end(); ){
        SerializeHelpers::indent(output_stream, m_indentation_counter); output_stream << "{\n";
        ++m_indentation_counter;

        write_branch_type(output_stream, neighbour_it->second);
        output_stream << ",\n";

        auto neighbour = m_board->get_view_from_node(neighbour_it->first);
        NodeSerializers::serialize_composition_node(output_stream, m_indentation_counter, neighbour_it->first.get());
        output_stream << ",\n";

        write_coords(output_stream, neighbour->x(), neighbour->y()); output_stream << '\n';
        --m_indentation_counter;
        SerializeHelpers::indent(output_stream, m_indentation_counter); output_stream << "}";

        if(++neighbour_it == neighbours.end()){
            output_stream << '\n';
        }else {
            output_stream << ",\n";
        }
    }

    --m_indentation_counter;
    SerializeHelpers::indent(output_stream, m_indentation_counter); output_stream << "]\n";
}

void DiagramJsonSerializer::serialize(std::ostream& output_stream){
    output_stream << "[\n";
    ++m_indentation_counter;

    auto m_diagram = m_board->get_diagram()->get_diagram();

    for (auto it = m_diagram.begin(); it != m_diagram.end(); ){
        SerializeHelpers::indent(output_stream, m_indentation_counter);
        ++m_indentation_counter;
        auto node_view = m_board->get_view_from_node(it->first);
        output_stream << "{\n";

        // 1) coords
        write_coords(output_stream, node_view->x(), node_view->y());
        output_stream << ",\n";

        // 2) node
        SerializeHelpers::write_indented_serialized_field<Composition>(output_stream, m_indentation_counter, "node", it->first.get(), &NodeSerializers::serialize_composition_node);
        output_stream << ",\n";

        // 3) neighbours (list of <{node, coords}, branch_type}>)
        serialize_neighbours(output_stream, it->second);

        --m_indentation_counter;
        SerializeHelpers::indent(output_stream, m_indentation_counter); output_stream << "}";

        if(++it == m_diagram.end()){
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

void DiagramJsonSerializer::deserialize(QJsonArray json_array){
    for(const auto& json_value : json_array){
        QJsonObject json_object = json_value.toObject();
        const auto [x,y] = deserialize_coords(json_object["coords"].toObject());

        std::shared_ptr<Composition> node = NodeSerializers::deserialize_composition_node(json_object["node"].toObject());
        m_board->add_item(node, x, y);

        const QJsonArray& neighbours = json_object["neighbours"].toArray();
        for (const auto& json_neighbour_val : neighbours){
            const auto& json_neighbour_object = json_neighbour_val.toObject();

            const auto [neighbour_x, neighbour_y] = deserialize_coords(json_object["coords"].toObject());
            const auto neighbour_node = NodeSerializers::deserialize_composition_node(json_neighbour_object["node"].toObject());
            const auto branch_type = get_branch_type_from_string(json_object["branch_type"].toString().toStdString());

            m_board->add_branch(node, neighbour_node, branch_type);
        }
    }
}

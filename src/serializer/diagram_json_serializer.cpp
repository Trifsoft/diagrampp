#include <serializer/diagram_json_serializer.h>
#include <model/base/uml_class_diagram_node.h>
#include <serializer/node_serializer/composition_serializer.h>
#include <serializer/node_serializer/enum_serializer.h>

/*
 * rules:
 * 1) every function is obligated to respect indentation
 * 2) no function print new line at the end ot json_stream
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

DiagramJsonSerializer::DiagramJsonSerializer(graph_type* m_diagram_graph, std::stringstream* stream, DiagramJsonSerializer::StreamMode sm) {
    m_diagram = m_diagram_graph;
    m_indentation_counter = 0;

    switch(sm){
        case DiagramJsonSerializer::StreamMode::READ:
            m_input_stream = stream;
            m_output_stream = nullptr;
            break;
        case DiagramJsonSerializer::StreamMode::WRITE:
            m_output_stream = stream;
            m_input_stream = nullptr;
            break;
    }
}

DiagramJsonSerializer::~DiagramJsonSerializer(){
    delete m_output_stream;
    delete m_input_stream;
}

void DiagramJsonSerializer::call_corresponding_node_serializer(IUMLClassDiagramNode* node){
    auto label = node->get_label().toStdString();

    if(label == "class" || label == "struct"){
        Composition* c = dynamic_cast<Composition*>(node);
        CompositionSerializer cs = CompositionSerializer(c, m_output_stream, m_input_stream, m_indentation_counter);

        cs.serialize();
    }else {
        CPPEnum* e = dynamic_cast<CPPEnum*>(node);
        EnumSerializer es = EnumSerializer(e, m_output_stream, m_input_stream, m_indentation_counter);

        es.serialize();
    }
}

void DiagramJsonSerializer::indent(){
    for (int i = 0; i < m_indentation_counter; i++){
        *m_output_stream << "\t";
    }
}

void DiagramJsonSerializer::trim_end(std::string& s) const {
    s.erase(
        std::find_if(s.rbegin(), s.rend(),[](unsigned char ch) { return !std::isspace(ch); }).base(),
        s.end()
    );
}

void DiagramJsonSerializer::write_coords(double x, double y){
    indent(); *m_output_stream << "\"coords\": {\n";
    ++m_indentation_counter;

    indent(); *m_output_stream << "\"x\": "; *m_output_stream << x << ',' << '\n';

    indent(); *m_output_stream << "\"y\": "; *m_output_stream << y << '\n';

    --m_indentation_counter;
    indent(); *m_output_stream << '}';
}

void DiagramJsonSerializer::write_inheritance(BranchType branch_type){
    indent();
    switch(branch_type){
        case BranchType::INHERITANCE:
            *m_output_stream << "\"branch_type\": " << "\"inheritance\"";
            break;
        case BranchType::ASSOCIATION:
            *m_output_stream << "\"branch_type\": " << "\"association\"";
            break;
        case BranchType::NAVIGATION:
            *m_output_stream << "\"branch_type\": " << "\"navigation\"";
            break;
        case BranchType::AGGREGATION:
            *m_output_stream << "\"branch_type\": " << "\"aggregation\"";
            break;
        case BranchType::COMPOSITION:
            *m_output_stream << "\"branch_type\": " << "\"composition\"";
            break;
        case BranchType::DEPENDENCY:
            *m_output_stream << "\"branch_type\": " << "\"dependency\"";
            break;
    }
}


std::string DiagramJsonSerializer::serialize(){
    int m_indentation_counter = 0;
    ;
    *m_output_stream << "[\n";
    ++m_indentation_counter;
    indent();

    for (auto it = m_diagram->begin(); it != m_diagram->end(); ){
        auto& node_view = it->first;
        *m_output_stream << "{\n";
        ++m_indentation_counter;

        // 1) coords
        write_coords(node_view->x(), node_view->y());
        *m_output_stream << ",\n";

        // 2) node
        const auto node = node_view->get_uml_class_diagram_node(); // does not return const IUMLClassDiagramNode
        call_corresponding_node_serializer(node);
        *m_output_stream << ",\n";

        // 3) neighbours (list of <{node, coords}, branch_type}>)
        *m_output_stream << "\"neighbours\":" << '[' << '\n';
        ++m_indentation_counter;
        for (auto neighbour_it = it->second.begin(); neighbour_it != it->second.end(); ){
            indent(); *m_output_stream << "{\n";
            ++m_indentation_counter;

            indent(); write_inheritance(neighbour_it->second); *m_output_stream << ",\n";


            const auto& neighbour = neighbour_it->first->get_uml_class_diagram_node();
            call_corresponding_node_serializer(neighbour); *m_output_stream << ",\n";

            indent(); write_coords(neighbour_it->first->x(), neighbour_it->first->y()); *m_output_stream << '\n';
            --m_indentation_counter;
            indent(); *m_output_stream << "}";

            if(++neighbour_it == it->second.end()){
                *m_output_stream << '\n';
            }else {
                *m_output_stream << ",\n";
            }
        }

        --m_indentation_counter;
        indent(); *m_output_stream << "]\n";

        --m_indentation_counter;
        indent(); *m_output_stream << "}";

        if(++it == m_diagram->end()){
            *m_output_stream << '\n';
        }else{
            *m_output_stream << ",\n";
        }
    }

    --m_indentation_counter;
    indent();
    *m_output_stream << "]\n";

    return (*m_output_stream).str();
}


DiagramJsonSerializer::deserialize(){

}

#ifndef GRAPH_PARSER_H
#define GRAPH_PARSER_H

#include <sstream>
#include <graph/diagram_graph.h>
/*
 * serialize:
 * Class is responsible for destructuring diagram into nodes and calling corresponding serializers
 *
 *
 * deserialize:
 * class is resposible for destructuring json, and making corresponding deserializers
 *
 */

class DiagramJsonSerializer{
private:
    int m_indentation_counter;

    // pointer -> copy c = deleted & stream does not remain const while reading with getline
    std::stringstream* m_output_stream;
    std::stringstream* m_input_stream;
    graph_type* m_diagram;

    void call_corresponding_node_serializer(IUMLClassDiagramNode* node);
    void indent();
    void trim_end(std::string& s) const;
    void write_node(std::stringstream json_stream);
    void write_coords(double x, double y);
    void write_inheritance(BranchType branch_type);
public:
    enum StreamMode{
        WRITE,
        READ
    };
    DiagramJsonSerializer(graph_type* diagram_graph, std::stringstream* stream, StreamMode sm);
    ~DiagramJsonSerializer();

    std::string serialize();
    // @throws invalid argument if input stream is not valid json format
    graph_type deserialize();
};

#endif // GRAPH_PARSER_H

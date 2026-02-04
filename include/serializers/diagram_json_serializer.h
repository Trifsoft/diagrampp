#ifndef DIAGRAM_JSON_SERIALIZER_H
#define DIAGRAM_JSON_SERIALIZER_H

#include <ostream>
#include <graph/diagram_graph.h>
#include <QJsonArray>
#include <Board.h>

/*
 * serialize function:
 * function is responsible for destructuring diagram into nodes and calling corresponding serializer
 *
 *
 * deserialize function:
 * function is resposible for destructuring json, and serves corresponding deserializer with json array or json object
 *
 */

class DiagramJsonSerializer{
private:
    int m_indentation_counter;
    Board* m_board;

    void call_corresponding_node_serializer(std::ostream& output_stream, IUMLClassDiagramNode* node) const;
    void trim_end(std::string& s) const;
    void write_coords(std::ostream& output_stream, double x, double y);
    void write_branch_type(std::ostream& output_stream, BranchType branch_type) const;

    std::pair<double, double> deserialize_coords(QJsonObject json_coords) const;
    BranchType deserialize_branch_type(QJsonValue json_branch_type) const;
    std::shared_ptr<Composition> deserialize_node(QJsonObject json_node) const;
public:
    DiagramJsonSerializer(Board*);
    ~DiagramJsonSerializer() = default;

    void serialize(std::ostream& output_stream);
    void deserialize(QJsonArray json_array);
};

#endif // DIAGRAM_JSON_SERIALIZER_H

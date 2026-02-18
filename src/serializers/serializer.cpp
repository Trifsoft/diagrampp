#include <serializers/serializer.h>
#include <serializers/diagram_json_serializer.h>

#include <fstream>

void Serializer::serialize(const QString &filePath, Board* board, DiagramGraph* diagram)
{
    std::ofstream outputStream(filePath.toStdString());
    if (!outputStream.is_open()) {
        emit serializationFailure("Failed to open file for writing: " + filePath);
        return;
    }

    DiagramJsonSerializer serializer(board, diagram);
    serializer.serialize(outputStream);

    outputStream.close();
    emit serializationSuccess(filePath);
}

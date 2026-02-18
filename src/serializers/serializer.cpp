#include <serializers/serializer.h>
#include <serializers/diagram_json_serializer.h>

void Serializer::serialize(const QString &filePath, Board* board)
{
    std::ofstream outputStream(filePath.toStdString());
    if (!outputStream.is_open()) {
        emit serializationFailure("Failed to open file for writing: " + filePath);
        return;
    }

    DiagramJsonSerializer serializer(board);
    serializer.serialize(outputStream);

    outputStream.close();
    emit serializationSuccess(filePath);
}

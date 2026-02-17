#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QObject>
#include <Board.h>

class Board;

class Serializer: public QObject {
    Q_OBJECT
private:
    Serializer() = default;
public:
    static Serializer& instance() {
        static Serializer inst;
        return inst;
    }
signals:
    void serializationSuccess(const QString& filePath);
    void serializationFailure(const std::string& errorMessage);
public slots:
    void serialize(const QString&, Board*);
};

#endif

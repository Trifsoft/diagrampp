#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <QString>

class Argument {
private:
    QString name;
    QString type;

public:
    Argument(const QString& name, const QString& type);
    Argument (const Argument &arg) = default;
    Argument() = default;

    QString get_name() const;
    QString get_type() const;

    QString to_string() const;
};

#endif // ARGUMENT_H

#ifndef CPP_ENUM_VALUE_H
#define CPP_ENUM_VALUE_H

#include <QString>
#include <optional>

class CPPEnumValue {
private:
    QString name;
    std::optional<int> value;

public:
    CPPEnumValue(const QString& name, std::optional<int> value = std::nullopt);

    QString get_name() const;
    std::optional<int> get_value() const;

    QString to_string() const;
};

#endif // CPP_ENUM_VALUE_H

#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H

#include <model/base/class_element.h>
#include <model/base/description.h>
#include <model/base/visibility.h>
#include <QString>
#include <QVector>
#include <optional>

class Constructor : public IClassElement {
protected:
    QString class_name;
    QVector<Description> arguments;

    virtual QString get_custom_definition() const = 0;

public:
    Constructor(const QString& class_name, const QVector<Description>& arguments, Visibility visibility);
    virtual ~Constructor() = default;

    QString get_declaration() const override;
    std::optional<QString> definition(const QString& class_name) const override;

    QVector<Description> get_arguments() const;
};

#endif // CONSTRUCTOR_H

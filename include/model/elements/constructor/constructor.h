#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H

#include <model/elements/class_element.h>
#include <model/elements/argument.h>
#include <model/base/visibility.h>
#include <QString>
#include <QVector>
#include <optional>

class Constructor : public IClassElement {
protected:
    QString class_name;
    QVector<Argument*> arguments;

    virtual QString get_custom_definition() const = 0;

public:
    Constructor(const QString& class_name, const QVector<Argument*>& arguments, Visibility visibility);
    virtual ~Constructor() = default;

    QString declaration() const override;
    std::optional<QString> definition(const QString& class_name) const override;

    QVector<Argument *> get_arguments() const;
};

#endif // CONSTRUCTOR_H

#ifndef COMPOSITION_ELEMENT_SERIALIZERS_H
#define COMPOSITION_ELEMENT_SERIALIZERS_H

#include <QJsonArray>
#include <QJsonObject>
#include <ostream>
#include <model/base/class_element.h>
#include <model/elements/field.h>
#include <model/elements/constructor/constructor.h>
#include <model/elements/destructor.h>
#include <model/elements/method.h>


/*
 * rules:
 * 1) every function is obligated to respect indentation, and print its content inside {} or []
 * 2) no function print new line at the end at passed output_stream, last char should be } or ]
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



namespace CompositionElementSerializers
{
    void serialize_method(std::ostream& output_stream, int indentation_counter, const Method* method);
    Method* deserialize_method(const QJsonObject json_method);

    void serialize_field(std::ostream& output_stream, int indentation_counter, const Field* field);
    Field* deserialize_field(const QJsonObject json_field);

    void serialize_constructor(std::ostream& output_stream, int indentation_counter, const Constructor* constructor);
    Constructor* deserialize_constructor(const QJsonObject json_constructor);

    void serialize_description(std::ostream& output_stream, int indentation_counter, const Description* description);
    Description* deserialize_description(const QJsonObject json_description);
};

#endif // COMPOSITION_ELEMENT_SERIALIZERS_H

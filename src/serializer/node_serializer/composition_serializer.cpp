#include <serializer/node_serializer/composition_serializer.h>
#include <model/elements/composition/cpp_class.h>
#include <serializer/composition_element_serializer/method_serializer.h>
#include <serializer/composition_element_serializer/field_serializer.h>
#include <serializer/composition_element_serializer/constructor_serializer.h>
#include <serializer/composition_element_serializer/destructor_serializer.h>

CompositionSerializer::CompositionSerializer(std::fstream* file_stream, const int indentation_counter, Composition* c)
    : NodeSerializer(file_stream, indentation_counter)
{
    m_composition_node = c;
}

void CompositionSerializer::serialize(){
    *m_file_stream << "{\n";
    ++m_indentation_counter;
    indent(); *m_file_stream << "\"label\": ";  *m_file_stream << '\"' << m_composition_node->get_label().toStdString() << "\",\n" ;
    indent(); *m_file_stream << "\"name\": "; *m_file_stream << m_composition_node->get_name().toStdString() << ",\n";

    // CONSTRUCTORS
    indent(); *m_file_stream << "\"constructors\": [\n";
    ++m_indentation_counter;

    auto& constructors = m_composition_node->constructors;
    for(auto constructor_it = constructors.begin(); constructor_it != constructors.end(); ){
        auto cs = ConstructorSerializer(m_file_stream, m_indentation_counter, constructor_it->get());
        cs.serialize();
        if(++constructor_it == constructors.end()){
            *m_file_stream << '\n';
        }else{
            *m_file_stream << ",\n";
        }
    }

    --m_indentation_counter;
    indent(); *m_file_stream << "], \n";


    // DESTRUCTOR
    indent(); *m_file_stream << "\"destructor\": {\n";
    ++m_indentation_counter;

    auto destructor = m_composition_node->get_destructor();
    auto ds = DestructorSerializer(m_file_stream, m_indentation_counter, destructor);
    ds.serialize();

    --m_indentation_counter;
    indent(); *m_file_stream << "},\n";



    // FIELDS
    indent(); *m_file_stream << "\"fields\": [\n";
    ++m_indentation_counter;

    auto& fields = m_composition_node->fields;
    for (auto field_it = fields.begin(); field_it != fields.end(); ){
        auto fs = FieldSerializer(m_file_stream, m_indentation_counter, field_it->get());
        fs.serialize();
        if(++field_it == fields.end()){
            *m_file_stream << '\n';
        }else{
            *m_file_stream << ",\n";
        }
    }

    --m_indentation_counter;
    indent(); *m_file_stream << "], \n";

    // METHODS
    indent(); *m_file_stream << "\"methods\": [\n";
    ++m_indentation_counter;

    auto& methods = m_composition_node->methods;
    for (auto method_it = methods.begin(); method_it != methods.end(); ){
        auto fs = MethodSerializer(m_file_stream, m_indentation_counter, method_it->get());
        fs.serialize();
        if(++method_it == methods.end()){
            *m_file_stream << '\n';
        }else{
            *m_file_stream << ",\n";
        }
    }
    --m_indentation_counter;
    indent(); *m_file_stream << "]\n";


    --m_indentation_counter;
    indent(); *m_file_stream << "}";
}

Composition* CompositionSerializer::deserialize(){
    return nullptr;
}

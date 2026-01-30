#include <serializer/node_serializer/composition_serializer.h>
#include <model/elements/composition/cpp_class.h>
#include <serializer/composition_element_serializer/method_serializer.h>
#include <serializer/composition_element_serializer/field_serializer.h>
#include <serializer/composition_element_serializer/constructor_serializer.h>
#include <serializer/composition_element_serializer/destructor_serializer.h>

CompositionSerializer::CompositionSerializer(Composition* c, std::stringstream* writing_stream, std::stringstream* reading_stream, const int indentation_counter)
    : NodeSerializer(writing_stream, reading_stream, indentation_counter)
{
    if(!writing_stream){ // deserialization
        m_composition_node = nullptr; // will be intialized ad made in deserialization function
    }else{
        m_composition_node = c;
    }
}

std::string CompositionSerializer::serialize(){
    *m_output_stream << "{\n";
    ++m_indentation_counter;
    indent(); *m_output_stream << "\"label\": ";  *m_output_stream << '\"' << m_composition_node->get_label().toStdString() << "\",\n" ;
    indent(); *m_output_stream << "\"name\": "; *m_output_stream << m_composition_node->get_name().toStdString() << ",\n";

    // CONSTRUCTORS
    indent(); *m_output_stream << "\"constructors\": [\n";
    ++m_indentation_counter;

    auto& constructors = m_composition_node->constructors;
    for(auto constructor_it = constructors.begin(); constructor_it != constructors.end(); ){
        auto cs = ConstructorSerializer(constructor_it->get(), m_output_stream, m_input_stream, m_indentation_counter);
        cs.serialize();
        if(++constructor_it == constructors.end()){
            *m_output_stream << '\n';
        }else{
            *m_output_stream << ",\n";
        }
    }

    --m_indentation_counter;
    indent(); *m_output_stream << "], \n";


    // DESTRUCTOR
    indent(); *m_output_stream << "\"destructor\": {\n";
    ++m_indentation_counter;

    auto destructor = m_composition_node->get_destructor();
    auto ds = DestructorSerializer(destructor, m_output_stream, m_input_stream, m_indentation_counter);
    ds.serialize();

    --m_indentation_counter;
    indent(); *m_output_stream << "},\n";



    // FIELDS
    indent(); *m_output_stream << "\"fields\": [\n";
    ++m_indentation_counter;

    auto& fields = m_composition_node->fields;
    for (auto field_it = fields.begin(); field_it != fields.end(); ){
        auto fs = FieldSerializer(field_it->get(), m_output_stream, m_input_stream, m_indentation_counter);
        fs.serialize();
        if(++field_it == fields.end()){
            *m_output_stream << '\n';
        }else{
            *m_output_stream << ",\n";
        }
    }

    --m_indentation_counter;
    indent(); *m_output_stream << "], \n";

    // METHODS
    indent(); *m_output_stream << "\"methods\": [\n";
    ++m_indentation_counter;

    auto& methods = m_composition_node->methods;
    for (auto method_it = methods.begin(); method_it != methods.end(); ){
        auto fs = MethodSerializer(method_it->get(), m_output_stream, m_input_stream, m_indentation_counter);
        fs.serialize();
        if(++method_it == methods.end()){
            *m_output_stream << '\n';
        }else{
            *m_output_stream << ",\n";
        }
    }
    --m_indentation_counter;
    indent(); *m_output_stream << "]\n";


    --m_indentation_counter;
    indent(); *m_output_stream << "}";

    return (*m_output_stream).str();
}

Composition* CompositionSerializer::deserialize(){
    return nullptr;
}

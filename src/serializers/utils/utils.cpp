#include <serializer/utils/utils.h>

void indent(std::ostream* output_stream, const int indentation_counter){
    for (int i = 0; i < indentation_counter; i++){
        *output_stream << "\t";
    }
}

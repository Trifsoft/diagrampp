#include "model/base/reference.h"

QString get_symbol(Reference reference) {
    switch(reference) {
        case(Reference::None): return "";
        case(Reference::LValue): return "&";
        case(Reference::RValue): return "&&";
    }
    return "";
}

#include <symengine/mp_class.h>

#if SYMENGINE_INTEGER_CLASS == SYMENGINE_FLINT
std::ostream &operator<<(std::ostream &os, const SymEngine::fmpz_wrapper &f) {
    char* c = fmpz_get_str(NULL, 10, f.get_fmpz_t());
    os << std::string(c);
    free(c);
    return os;
}

std::ostream &operator<<(std::ostream &os, const SymEngine::fmpq_wrapper &f) {
    char* c = fmpq_get_str(NULL, 10, f.get_fmpq_t());
    os << std::string(c);
    free(c);
    return os;
}
#endif


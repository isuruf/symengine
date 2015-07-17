#include <cstddef>
#include <stdexcept>

#include <symengine/symengine_rcp.h>
#include "Teuchos_RCP.hpp"

namespace SymEngine {

#if defined(WITH_SYMENGINE_RCP)


void print_stack_on_segfault()
{
#if defined(WITH_SYMENGINE_STACKTRACE)
    Teuchos::print_stack_on_segfault();
#endif
}

#endif

} // SymEngine

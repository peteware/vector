#include <pw_cstddef.h>

#include <typeinfo>

namespace pw {
static_assert (sizeof(size_t) == sizeof(void*));
}

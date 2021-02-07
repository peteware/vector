#include <pw/cstddef>

#include <typeinfo>

namespace pw {
static_assert(sizeof(size_t) == sizeof(void*));
}

#include <pw_cstddef.h>

#include <typeinfo>

namespace pw {
static_assert (typeid(size_t) == typeid(sizeof(char)));
}

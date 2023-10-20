#ifndef PW_IMPL_INITIALIZER_LIST_H
#define PW_IMPL_INITIALIZER_LIST_H

#include <initializer_list>

namespace pw {
template<class Type>
using initializer_list = std::initializer_list<Type>;
} // namespace pw

#endif /* PW_IMPL_INITIALIZER_LIST_H */

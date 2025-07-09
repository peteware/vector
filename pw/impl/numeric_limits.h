#ifndef INCLUDED_PW_IMPL_NUMERIC_LIMITS_H
#define INCLUDED_PW_IMPL_NUMERIC_LIMITS_H
#include <limits>

namespace pw {
/*
 * There's just too much work with little gain to implement our own
 * numeric_limits<> so juse use the std:: one.  It's unfortunate
 * that drags in all of <limits>.
 */
template<class Type>
using numeric_limits = std::numeric_limits<Type>;
} // namespace pw
#endif // INCLUDED_PW_IMPL_NUMERIC_LIMITS_H

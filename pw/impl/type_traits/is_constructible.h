#ifndef INCLUDED_PW_IMPL_IS_CONSTRUCTIBLE_H
#define INCLUDED_PW_IMPL_IS_CONSTRUCTIBLE_H

#include <pw/impl/type_traits/void.h>
#include <pw/internal/constructible.h>

namespace pw {

template<class Type, class... Args>
using is_constructible = internal::constructible<void_t<>, Type, Args...>;

template<class Type, class... Args>
inline constexpr bool is_constructible_v = is_constructible<Type, Args...>::value;
} // namespace pw

#endif /* INCLUDED_PW_IMPL_IS_CONSTRUCTIBLE_H */

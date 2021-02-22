#ifndef INCLUDED_PW_IMPL_IS_CONSTRUCTIBLE_H
#define INCLUDED_PW_IMPL_IS_CONSTRUCTIBLE_H

#include <pw/impl/bool_type.h>

namespace pw {
template<class Type, class... Args>
struct is_constructible : public false_type
{
};

template<class Type, class... Args>
inline constexpr bool is_constructible_v = is_constructible<Type, Args...>::value;
} // namespace pw

#endif /* INCLUDED_PW_IMPL_IS_CONSTRUCTIBLE_H */

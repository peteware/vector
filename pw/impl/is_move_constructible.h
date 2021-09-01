#ifndef INCLUDED_PW_IMPL_IS_MOVE_CONSTRUCTIBLE_H
#define INCLUDED_PW_IMPL_IS_MOVE_CONSTRUCTIBLE_H

#include <pw/impl/integral_constant.h>

namespace pw {

template<class Type, class... Args>
struct is_nothrow_constructible : public integral_constant<bool, __is_nothrow_constructible(Type, Args...)>
{
};

template<class Type, class... Args>
inline constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<Type, Args...>::value;

} // namespace pw

#endif /* INCLUDED_PW_IMPL_IS_MOVE_CONSTRUCTIBLE_H */

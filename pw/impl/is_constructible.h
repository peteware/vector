#ifndef INCLUDED_PW_IMPL_IS_CONSTRUCTIBLE_H
#define INCLUDED_PW_IMPL_IS_CONSTRUCTIBLE_H

#include <pw/impl/bool_type.h>
#include <pw/impl/void.h>

namespace pw {

#if 0
template<class T>
typename add_rvalue_reference<T>::type declval() noexcept;

template<class, class Type, class... Args>
struct is_constructible_ : pw::false_type
{
};

template<class Type, class... Args>
struct is_constructible_<void_t<decltype(Type(pw::declval<Args>()...))>, Type, Args...> : pw::true_type
{
};
template<class Type, class... Args>
using is_constructible = is_constructible_<void_t<>, Type, Args...>;

template<class Type, class... Args>
inline constexpr bool is_constructible_v = is_constructible<Type, Args...>::value;
#endif
} // namespace pw

#endif /* INCLUDED_PW_IMPL_IS_CONSTRUCTIBLE_H */

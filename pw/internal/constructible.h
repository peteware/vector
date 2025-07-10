#ifndef INCLUDED_PW_INTERNAL_CONSTRUCTIBLE_H
#define INCLUDED_PW_INTERNAL_CONSTRUCTIBLE_H

#include <pw/impl/bool_type.h>
#include <pw/impl/declval.h>
#include <pw/impl/void.h>

namespace pw::internal {
template<class, class Type, class... Args>
struct constructible : pw::false_type
{
};

template<class Type, class... Args>
struct constructible<void_t<decltype(Type(pw::declval<Args>()...))>, Type, Args...> : pw::true_type
{
};
} // namespace pw::internal

#endif /* INCLUDED_PW_INTERNAL_CONSTRUCTIBLE_H */

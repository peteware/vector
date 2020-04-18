#ifndef INCLUDED_PW_INTERNAL_IS_SUPPORTED_H
#define INCLUDED_PW_INTERNAL_IS_SUPPORTED_H

#include <pw/impl/bool.h>
#include <pw/impl/void.h>

namespace pw { namespace internal {

/*
 * This is based on code from the article:
 *
 *    https://neatloop.dev/posts/2019-05-19-member_detection_part_2/
 *
 * Usage to check if a member function exists is:
 * @code
 * template <typename T>
 * using to_string_pred = decltype(std::declval<T>().());

 * @endcode
 */
template<template<typename> typename Predicate, typename Type, typename = pw::void_t<>>
struct supports : std::false_type
{
};

template<template<typename> typename Predicate, typename Type>
struct supports<Predicate, Type, std::void_t<Predicate<Type>>> : pw::true_type
{
};

template<template<typename> typename Predicate, typename Type>
using is_supported = supports<Predicate, Type>;

template<template<typename> typename Predicate, typename Type>
static constexpr auto is_supported_v = is_supported<Predicate, Type>::value;

}} // namespace pw::internal
#endif /* INCLUDED_PW_INTERNAL_IS_SUPPORTED_H */

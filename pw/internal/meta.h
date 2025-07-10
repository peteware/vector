#ifndef INCLUDED_PW_INTERNAL_META_H
#define INCLUDED_PW_INTERNAL_META_H

// * Meta
// ** Includes
// *** pw includes
#include <pw/impl/ptrdiff.h>
#include <pw/impl/void.h>
#include <pw/internal/extract_or.h>

namespace pw::internal {

template<typename Type>
using get_const_void_pointer = typename Type::const_void_pointer;

template<typename Type>
using get_difference_type = typename Type::difference_type;

template<class Type, class = void>
struct difference_type
{
    using type = extract_or_type<get_difference_type, Type, ptrdiff_t>;
};

template<typename Type>
struct get_first_arg;

template<template<typename...> class Class, typename Type, typename... Args>
struct get_first_arg<Class<Type, Args...>>
{
    using type = Type;
};

template<class Type, class = void>
struct element_type
{
    using type = typename get_first_arg<Type>::type;
};

template<typename Type>
struct element_type<Type, void_t<typename Type::element_type>>
{
    using type = typename Type::element_type;
};

template<typename Type, typename U>
struct rebind_first_arg;

template<template<typename...> class Class, typename Type, typename... Args, typename U>
struct rebind_first_arg<Class<Type, Args...>, U>
{
    using type = Class<U, Args...>;
};

template<typename Type, typename U, typename = void>
struct rebind_ptr
{
    using type = typename rebind_first_arg<Type, U>::type;
};

template<typename Type, typename U>
struct rebind_ptr<Type, U, void_t<typename Type::template rebind<U>>>
{
    using type = typename Type::template rebind<U>;
};

} // End of namespace pw::internal

#endif /* INCLUDED_PW_INTERNAL_META_H */

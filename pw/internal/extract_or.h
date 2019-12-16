#ifndef INCLUDED_PW_INTERNAL_EXTRACT_OR_H
#define INCLUDED_PW_INTERNAL_EXTRACT_OR_H

namespace pw { namespace internal {

template<template<typename> class Extract, typename Obj, typename Default, typename>
struct extract_or
{
    using type = Default;
};

template<template<typename> class Extract, typename Obj, typename Default>
struct extract_or<Extract, Obj, Default, void_t<Extract<Obj> > >
{
    using type = Extract<Obj>;
};

template<template<typename> class Extract, typename Obj, typename Default>
using extract_or_type = typename extract_or<Extract, Obj, Default, void>::type;

}} // namespace pw::internal

#endif /*  INCLUDED_PW_INTERNAL_EXTRACT_OR_H */

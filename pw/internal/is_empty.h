#ifndef INCLUDED_PW_INTERNAL_IS_EMPTY_H
#define INCLUDED_PW_INTERNAL_IS_EMPTY_H

#include <pw/internal/is_class.h>
#include <pw/internal/integral_constant.h>

namespace pw { namespace internal {

template<class Type>
struct is_empty1 : public Type
{
    double m_dummy;
};

struct is_empty2
{
    double m_dummy;
};

template<class Type, bool = is_class<Type>::value>
struct is_empty : public integral_constant<bool, sizeof(is_empty1<Type>) == sizeof(is_empty2)>
{
};

template<class Type>
struct is_empty<Type, false> : public false_type
{
};

}} // namespace pw::internal

#endif /* INCLUDED_PW_INTERNAL_IS_EMPTY_H */

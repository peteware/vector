#ifndef INCLUDED_PW_INTERNAL_IS_EMPTY_H
#define INCLUDED_PW_INTERNAL_IS_EMPTY_H

#include <pw/impl/bool_type.h>
#include <pw/impl/integral_constant.h>
#include <pw/impl/is_class.h>

namespace pw::internal {

template<class Type>
struct check_empty1 : public Type
{
    double m_dummy;
};

struct check_empty2
{
    double m_dummy;
};
} // namespace pw::internal

namespace pw {

template<class Type, bool = is_class<Type>::value>
struct is_empty
    : public integral_constant<bool, sizeof(internal::check_empty1<Type>) == sizeof(internal::check_empty2)>
{
};

template<class Type>
struct is_empty<Type, false> : public false_type
{
};

template<class Type>
inline constexpr bool is_empty_v = is_empty<Type>::value;

} // namespace pw
#endif /* INCLUDED_PW_INTERNAL_IS_EMPTY_H */

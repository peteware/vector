#ifndef INCLUDED_PW_INTERNAL_IS_EMPTY_H
#define INCLUDED_PW_INTERNAL_IS_EMPTY_H

#include <pw/impl/integral_constant.h>
#include <pw/impl/is_class.h>
#include <pw/internal/check_empty.h>

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

} // namespace pw
#endif /* INCLUDED_PW_INTERNAL_IS_EMPTY_H */

#ifndef INCLUDED_PW_INTERNAL_IS_SAME_H
#define INCLUDED_PW_INTERNAL_IS_SAME_H

#include <pw/internal/bool_type.h>

namespace pw { namespace internal {

template<class Type, class U>
struct is_same : false_type
{
};

template<class Type>
struct is_same<Type, Type> : true_type
{
};

}} // namespace pw::internal

#endif /* INCLUDED_PW_INTERNAL_IS_SAME_H */

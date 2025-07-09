#ifndef INCLUDED_PW_IMPL_IS_SAME_H
#define INCLUDED_PW_IMPL_IS_SAME_H

#include <pw/impl/bool_type.h>

namespace pw {

template<class Type, class U>
struct is_same : false_type
{
};

template<class Type>
struct is_same<Type, Type> : true_type
{
};

template<class Type, class U>
inline constexpr bool is_same_v = is_same<Type, U>::value;
} // namespace pw

#endif /* INCLUDED_PW_IMPL_IS_SAME_H */

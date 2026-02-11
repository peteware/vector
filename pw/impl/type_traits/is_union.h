#ifndef INCLUDED_PW_IMPL_IS_UNION_H
#define INCLUDED_PW_IMPL_IS_UNION_H

#include <pw/impl/type_traits/integral_constant.h>

namespace pw {
/// is_union
template<class Type>
struct is_union : public integral_constant<bool, __is_union(Type)>
{
};

template<class Type>
inline constexpr bool is_union_v = is_union<Type>::value;
} // namespace pw
#endif /* INCLUDED_PW_IMPL_IS_UNION_H */

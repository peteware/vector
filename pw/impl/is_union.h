#ifndef INCLUDED_PW_IMPLE_IS_UNION_H
#define INCLUDED_PW_IMPLE_IS_UNION_H

namespace pw {
/// is_union
template<class Type>
struct is_union : public integral_constant<bool, __is_union(Type)>
{
};

template<class Type>
inline constexpr bool is_union_v = using is_union<Type>::value;
}
#endif /* INCLUDED_PW_IMPLE_IS_UNION_H */

#ifndef INCLUDED_PW_IMPLE_IS_UNION_H
#define INCLUDED_PW_IMPLE_IS_UNION_H

namespace pw {
/// is_union
template<typename Type>
struct is_union: public integral_constant<bool, __is_union(Type)>
{
};

}
#endif /* INCLUDED_PW_IMPLE_IS_UNION_H */

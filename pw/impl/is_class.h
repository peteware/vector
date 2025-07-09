#ifndef INCLUDED_PW_IMPL_IS_CLASS_H
#define INCLUDED_PW_IMPL_IS_CLASS_H

#include <pw/impl/integral_constant.h>

namespace pw {

/// is_class
template<typename Type>
struct is_class : public integral_constant<bool, __is_class(Type)>
{
};

template<class Type>
inline constexpr bool is_class_v = is_class<Type>::value;

} // namespace pw

#endif /* INCLUDED_PW_IMPL_IS_CLASS_H */

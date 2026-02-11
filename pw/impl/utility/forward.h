#ifndef INCLUDED_PW_IMPL_FORWARD_H
#define INCLUDED_PW_IMPL_FORWARD_H

#include <pw/impl/type_traits/remove_reference.h>

namespace pw {

template<typename Type>
constexpr Type&&
forward(typename remove_reference<Type>::type& value) noexcept
{
    return static_cast<Type&&>(value);
}

template<typename Type>
constexpr Type&&
forward(typename remove_reference<Type>::type&& value) noexcept
{
    return static_cast<Type&&>(value);
}

} // namespace pw
#endif /*  INCLUDED_PW_IMPL_FORWARD_H */

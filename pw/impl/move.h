#ifndef INCLUDED_PW_IMPL_MOVE_H
#define INCLUDED_PW_IMPL_MOVE_H

#include <pw/impl/remove_reference.h>

namespace pw {

template<class Type>
constexpr typename pw::remove_reference<Type>::type&&
move(Type&& value) noexcept
{
    return static_cast<typename pw::remove_reference<Type>::type&&>(value);
}

} // namespace pw

#endif /*  INCLUDED_PW_IMPL_MOVE_H */

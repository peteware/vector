#ifndef INCLUDED_AS_CONST_H
#define INCLUDED_AS_CONST_H

#include <pw/impl/add_const.h>

namespace pw {

template<class Type>
constexpr add_const_t<Type>&
as_const(Type& value) noexcept
{
    return value;
}

template<class Type>
void as_const(Type const&&) = delete;
} // namespace pw

#endif /* INCLUDED_AS_CONST_H */

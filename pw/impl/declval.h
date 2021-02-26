#ifndef INCLUDED_PW_IMPL_DECLVAL_H
#define INCLUDED_PW_IMPL_DECLVAL_H

#include <pw/impl/bool_type.h>
namespace pw {

template<typename Type>
Type
declval() noexcept
{
    static_assert(sizeof(Type) == 0, "Do not instantiate");
}

} // namespace pw

#endif /*  INCLUDED_PW_IMPL_DECLVAL_H */

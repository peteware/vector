#ifndef INCLUDED_PW_IMPL_EXCHANGE_H
#define INCLUDED_PW_IMPL_EXCHANGE_H

#include <pw/impl/forward.h>
#include <pw/impl/move.h>

namespace pw {

template<class Type, class OtherType = Type>
constexpr Type
exchange(Type& obj, OtherType&& new_value)
{
    Type old = pw::move(obj);
    obj      = forward<OtherType>(new_value);
    return old;
}

} // namespace pw

#endif /* INCLUDED_PW_IMPL_EXCHANGE_H */

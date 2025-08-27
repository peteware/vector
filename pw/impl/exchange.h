#ifndef INCLUDED_PW_IMPL_EXCHANGE_H
#define INCLUDED_PW_IMPL_EXCHANGE_H

#include <pw/impl/forward.h>
#include <pw/impl/move.h>

namespace pw {

/**
 * @brief Replaces the value of obj with new_value and returns the old value of obj.
 * @tparam Type
 * @tparam OtherType (defaults to Type) - allows for different but convertible types
 * @param obj The object whose value is to be replaced.
 * @param new_value The new value to assign to obj.
 * @return The old value of obj before the exchange.
 */
template<class Type, class OtherType = Type>
constexpr Type
exchange(Type& obj, OtherType&& new_value)
{
    Type old = pw::move(obj);
    obj      = pw::forward<OtherType>(new_value);
    return old;
}

} // namespace pw

#endif /* INCLUDED_PW_IMPL_EXCHANGE_H */

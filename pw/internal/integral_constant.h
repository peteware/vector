#ifndef INCLUDED_PW_INTERNAL_INTEGRAL_CONSTANT_H
#define INCLUDED_PW_INTERNAL_INTEGRAL_CONSTANT_H

namespace pw { namespace internal {

template<class Type, Type v>
struct integral_constant
{
    using value_type                  = Type;
    using type                        = integral_constant;
    static constexpr value_type value = v;

    constexpr            operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

}} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_INTEGRAL_CONSTANT_H */

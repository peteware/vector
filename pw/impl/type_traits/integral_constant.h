#ifndef INCLUDED_PW_IMPL_INTEGRAL_CONSTANT_H
#define INCLUDED_PW_IMPL_INTEGRAL_CONSTANT_H

namespace pw {

template<class Type, Type v>
struct integral_constant
{
    using value_type                  = Type;
    using type                        = integral_constant;
    static constexpr value_type value = v;

    explicit constexpr          operator value_type() const noexcept { return value; }
    constexpr value_type        operator()() const noexcept { return value; }
};

} // namespace pw
#endif /*  INCLUDED_PW_IMPL_INTEGRAL_CONSTANT_H */

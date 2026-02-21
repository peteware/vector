#ifndef INCLUDED_PW_IMPL_INTEGRAL_CONSTANT_H
#define INCLUDED_PW_IMPL_INTEGRAL_CONSTANT_H

#include <type_traits>
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

// Provide conversions to std::true_type and std::false_type for compatibility with std::vector
namespace pw {

// Explicit specializations for bool to allow std::vector to work with our integral_constant
template<>
struct integral_constant<bool, true>
{
    using value_type                  = bool;
    using type                        = integral_constant;
    static constexpr value_type value = true;

    explicit constexpr          operator value_type() const noexcept { return value; }
    constexpr value_type        operator()() const noexcept { return value; }

    // Conversion to std::true_type
    constexpr operator ::std::true_type() const noexcept { return ::std::true_type(); }
};

template<>
struct integral_constant<bool, false>
{
    using value_type                  = bool;
    using type                        = integral_constant;
    static constexpr value_type value = false;

    explicit constexpr          operator value_type() const noexcept { return value; }
    constexpr value_type        operator()() const noexcept { return value; }

    // Conversion to std::false_type
    constexpr operator ::std::false_type() const noexcept { return ::std::false_type(); }
};

} // namespace pw

#endif /*  INCLUDED_PW_IMPL_INTEGRAL_CONSTANT_H */

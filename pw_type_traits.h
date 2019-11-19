#ifndef PW_TYPE_TRAITS_H
#define PW_TYPE_TRAITS_H

namespace pw {

template<class Type, Type v>
struct integral_constant {
    using value_type = Type;
    using type = integral_constant;
    static constexpr value_type value = v;

    constexpr operator value_type() const noexcept
    {
        return value;
    }
    constexpr value_type operator()() const noexcept
    {
        return value;
    }
};

using true_type	 = pw::integral_constant<bool, true>;
using false_type = pw::integral_constant<bool, false>;

}
#endif /*  PW_TYPE_TRAITS_H */

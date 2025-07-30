#ifndef INCLUDED_PW_IMPL_IS_CONSTANT_EVALUATED_H
#define INCLUDED_PW_IMPL_IS_CONSTANT_EVALUATED_H

namespace pw {
inline constexpr bool
is_constant_evaluated() noexcept
{
    return __builtin_is_constant_evaluated();
}
} // namespace pw

#endif //INCLUDED_PW_IMPL_IS_CONSTANT_EVALUATED_H

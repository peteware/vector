#ifndef INCLUDED_PW_IMPL_IS_CONVERTIBLE_H
#define INCLUDED_PW_IMPL_IS_CONVERTIBLE_H

namespace pw {
template<class From, class To>
struct is_convertible;

template<class From, class To>
inline constexpr bool is_convertible_v = is_convertible<From, To>::value;
} // namespace pw

#endif /*  INCLUDED_PW_IMPL_IS_CONVERTIBLE_H */

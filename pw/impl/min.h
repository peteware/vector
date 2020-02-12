#ifndef INCLUDED_PW_IMPL_MIN_H
#define INCLUDED_PW_IMPL_MIN_H

namespace pw {

template<class Type>
constexpr Type const&
min(Type const& a, Type const& b)
{
    return a < b ? a : b;
}

} // namespace pw
#endif /*  INCLUDED_PW_IMPL_MIN_H */

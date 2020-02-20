#ifndef INCLUDED_PW_IMPL_MIN_H
#define INCLUDED_PW_IMPL_MIN_H

namespace pw {

template<class Type>
constexpr Type const&
min(Type const& a, Type const& b)
{
    return (a < b) ? a : b;
}

template<class Type, class Compare>
constexpr Type const&
min(Type const& op1, Type const& op2, Compare compare)
{
    return (compare(op1, op2)) ? op1 : op2;
}

} // namespace pw
#endif /*  INCLUDED_PW_IMPL_MIN_H */

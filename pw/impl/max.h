#ifndef INCLUDED_PW_IMPL_MAX_H
#define INCLUDED_PW_IMPL_MAX_H
namespace pw {

template<class Type>
constexpr Type const&
max(Type const& op1, Type const& op2)
{
    return (op1 < op2) ? op2 : op1;
}

template<class Type, class Compare>
constexpr Type const&
max(Type const& op1, Type const& op2, Compare compare)
{
    return (compare(op1, op2)) ? op2 : op1;
}

} // namespace pw
#endif /*  INCLUDED_PW_IMPL_MAX_H */

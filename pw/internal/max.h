#ifndef INCLUDED_PW_INTERNAL_MAX_H
#define INCLUDED_PW_INTERNAL_MAX_H
namespace pw { namespace internal {

template<class Type>
Type const&
max(Type const& op1, Type const& op2)
{
    return (op1 < op2) ? op2 : op1;
}

template<class Type, class Compare>
Type const&
max(Type const& op1, Type const& op2, Compare compare)
{
    return (compare(op1, op2)) ? op2 : op1;
}

}} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_MAX_H */

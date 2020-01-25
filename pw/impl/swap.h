#ifndef INCLUDED_PW_IMPL_SWAP_H
#define INCLUDED_PW_IMPL_SWAP_H
namespace pw {

template<class Type>
void
swap(Type& op1, Type& op2)
{
    Type tmp(op1);

    op1 = op2;
    op2 = tmp;
}

} // namespace pw
#endif /* INCLUDED_PW_IMPL_SWAP_H */

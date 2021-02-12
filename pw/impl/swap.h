#ifndef INCLUDED_PW_IMPL_SWAP_H
#define INCLUDED_PW_IMPL_SWAP_H
#include <iostream>
#include <pw/impl/move.h>

namespace pw {

template<class Type>
void
swap(Type& op1, Type& op2)
{
    Type tmp(pw::move(op1));

    op1 = pw::move(op2);
    op2 = pw::move(tmp);
}

} // namespace pw
#endif /* INCLUDED_PW_IMPL_SWAP_H */

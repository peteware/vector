#ifndef INCLUDED_PW_IMPL_SWAP_H
#define INCLUDED_PW_IMPL_SWAP_H

#include <pw/impl/utility/move.h>

namespace pw {

/**
 * Exchanges the values of two objects using move semantics.
 *
 * Swaps the values of op1 and op2 by moving their contents. This function
 * uses pw::move() to efficiently exchange the values, minimizing copies.
 *
 * @tparam Type The type of the objects to swap. Must be MoveConstructible and MoveAssignable.
 * @param op1 First object to swap.
 * @param op2 Second object to swap.
 *
 */
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

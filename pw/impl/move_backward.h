#ifndef INCLUDED_PW_IMPL_MOVE_BACKWARD_H
#define INCLUDED_PW_IMPL_MOVE_BACKWARD_H

#include <pw/impl/move.h>

namespace pw {

/**
 * Moves the elements from the range [begin, end) to another range ending at dest.
 * The elements are moved in reverse order (the last element is moved first), 
 * but their relative order is preserved.
 *
 * @param begin Input iterator to the first element of the range to move from
 * @param end   Input iterator to one past the last element of the range to move from  
 * @param dest  Output iterator to one past the last element of the destination range
 *
 * @return Iterator to the element that would precede the first element of the 
 *         destination range (i.e., dest - (end - begin))
 *
 * @note The ranges [begin, end) and [dest - (end - begin), dest) must not overlap,
 *       or if they overlap, dest must not be in the range [begin, end).
 */
template<class Iterator1, class Iterator2>
Iterator2
move_backward(Iterator1 begin, Iterator1 end, Iterator2 dest)
{
    while (begin != end)
    {
        *(--dest) = pw::move(*(--end));
    }
    return dest;
}
} // namespace pw
#endif /* INCLUDED_PW_IMPL_MOVE_BACKWARD_H */

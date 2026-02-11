#ifndef INCLUDED_PW_IMPL_MOVE_ALG_H
#define INCLUDED_PW_IMPL_MOVE_ALG_H

#include <pw/impl/utility/move.h>

namespace pw {

/**
 * Moves the elements in the range [begin, end) to another range beginning at dest.
 * The elements are moved in order (the first element is moved first).
 *
 * @param begin Input iterator to the first element of the range to move from
 * @param end   Input iterator to one past the last element of the range to move from
 * @param dest  Output iterator to the beginning of the destination range
 *
 * @return Output iterator to the element in the destination range, one past
 *         the last element moved (i.e., dest + (end - begin))
 *
 * @note The ranges [begin, end) and [dest, dest + (end - begin)) must not overlap,
 *       or if they overlap, dest must not be in the range (begin, end].
 */
template<class InputIterator, class OutputIterator>
OutputIterator
move(InputIterator begin, InputIterator end, OutputIterator dest)
{
    while (begin != end)
    {
        *dest++ = move(*begin++);
    }
    return dest;
}
} // namespace pw

#endif /*  INCLUDED_PW_IMPL_MOVE_ALG_H */

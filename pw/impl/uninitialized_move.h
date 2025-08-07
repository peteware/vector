#ifndef INCLUDED_PW_IMPL_UNINITIALIZED_MOVE_CONSTRUCT_H
#define INCLUDED_PW_IMPL_UNINITIALIZED_MOVE_CONSTRUCT_H

#include <pw/impl/addressof.h>
#include <pw/impl/construct_at.h>
#include <pw/impl/destroy.h>
#include <pw/impl/move.h>

namespace pw {

/**
 * @brief Moves a range of objects to uninitialized memory
 *
 * Constructs objects in the uninitialized storage starting at 'out' by moving elements
 * from the range [begin, end). For each iterator i in [out, out + (end - begin)),
 * move-constructs an object at that location using the value from the corresponding
 * input iterator. The source objects are left in a valid but unspecified state.
 *
 * @tparam InputIterator Type of the input iterators
 * @tparam OutputIterator Type of the output iterator
 *
 * @param begin Iterator to the first element in the source range
 * @param end Iterator to one past the last element in the source range
 * @param out Iterator to the start of the uninitialized destination range
 *
 * @throws Any exception thrown by the element type's move constructor.
 *         Strong exception guarantee: if an exception is thrown during the
 *         initialization of any element, all previously constructed elements
 *         are destroyed.  The input range is left in a valid but
 *         unspecified state.
 *
 * @note After the operation, elements in the source range will be in a valid but
 *       unspecified state, as they have been moved from.
 */
template<class InputIterator, class OutputIterator>
void
uninitialized_move(InputIterator begin, InputIterator end, OutputIterator out)
{
    OutputIterator current = out;

    try
    {
        while (begin != end)
        {
            pw::construct_at(pw::addressof(*current), pw::move(*begin));
            ++current;
            ++begin;
        }
    }
    catch (...)
    {
        pw::destroy(out, current);
        throw;
    }
}
} // namespace pw
#endif /*  INCLUDED_PW_IMPL_UNINITIALIZED_MOVE_CONSTRUCT_H */

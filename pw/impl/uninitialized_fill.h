#ifndef INCLUDED_PW_IMPL_UNINITIALIZED_FILL_CONSTRUCT_H
#define INCLUDED_PW_IMPL_UNINITIALIZED_FILL_CONSTRUCT_H

#include <pw/impl/addressof.h>
#include <pw/impl/construct_at.h>
#include <pw/impl/destroy.h>

namespace pw {

/**
 * @brief Fills uninitialized memory range with copies of a given value
 *
 * Constructs objects of type Type in the uninitialized storage designated by the range
 * [begin, end) by copy-constructing value in each location. This function assumes the
 * memory is uninitialized and does not call destructors of existing objects.
 *
 * @tparam Iterator Type of the iterators
 * @tparam Type Type of the value being constructed
 *
 * @param begin Iterator to the start of the uninitialized storage range
 * @param end Iterator to the end of the uninitialized storage range
 * @param value The value to copy-construct in each location
 *
 * @throws Any exception thrown by Type's copy constructor.
 *         Strong exception guarantee: if an exception is thrown, all constructed
 *         elements are destroyed and the function has no effect.
 */
template<class Iterator, class Type>
void
uninitialized_fill(Iterator begin, Iterator end, Type const& value)
{
    Iterator current = begin;
    try
    {
        while (current != end)
        {
            pw::construct_at(pw::addressof(*current), value);
            ++current;
        }
    }
    catch (...)
    {
        pw::destroy(begin, current);
        throw;
    }
}
} // namespace pw
#endif /*  INCLUDED_PW_IMPL_UNINITIALIZED_FILL_CONSTRUCT_H */

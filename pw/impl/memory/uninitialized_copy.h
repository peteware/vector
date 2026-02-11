#ifndef INCLUDED_PW_IMPL_UNINITIALIZED_COPY_CONSTRUCT_H
#define INCLUDED_PW_IMPL_UNINITIALIZED_COPY_CONSTRUCT_H

#include <pw/impl/construct_at.h>
#include <pw/impl/memory/addressof.h>
#include <pw/impl/memory/destroy.h>

namespace pw {

/**
 * @brief Copies a range of objects to uninitialized memory
 *
 * Constructs objects in the uninitialized storage starting at 'out' by copying elements
 * from the range [begin, end). For each iterator i in [out, out + (end - begin)),
 * constructs an object at that location using the value *j, where j is the corresponding
 * input iterator.
 *
 * @tparam InputIterator Type of the input iterators
 * @tparam OutputIterator Type of the output iterator
 *
 * @param begin Iterator to the first element in the source range
 * @param end Iterator to one past the last element in the source range
 * @param out Iterator to the start of the uninitialized destination range
 *
 * @throws Any exception thrown by the element type's copy constructor.
 *         Strong exception guarantee: if an exception is thrown during the
 *         initialization of any element, all previously constructed elements
 *         are destroyed and the function has no effect.
 */
template<class InputIterator, class OutputIterator>
constexpr void
uninitialized_copy(InputIterator begin, InputIterator end, OutputIterator out)
{
    OutputIterator current = out;

    try
    {
        while (begin != end)
        {
            pw::construct_at(pw::addressof(*current), *begin);
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
#endif /*  INCLUDED_PW_IMPL_UNINITIALIZED_COPY_CONSTRUCT_H */

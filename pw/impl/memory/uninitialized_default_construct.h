#ifndef INCLUDED_PW_IMPL_UNINITIALIZED_DEFAULT_CONSTRUCT_H
#define INCLUDED_PW_IMPL_UNINITIALIZED_DEFAULT_CONSTRUCT_H

#include <pw/impl/memory/addressof.h>
#include <pw/impl/memory/construct_at.h>
#include <pw/impl/memory/destroy.h>

namespace pw {

/**
 * @brief Default-constructs objects in uninitialized storage in the given range.
 *
 * This function constructs objects of the type `Value` in the uninitialized memory
 * pointed to by the range [begin, end). Construction is performed using the
 * default constructor. If an exception is thrown during construction, all
 * objects that were already constructed are destroyed using `pw::destroy`.
 *
 * @tparam Iterator Iterator type pointing to uninitialized storage.
 * @param begin Iterator to the beginning of the range to construct.
 * @param end Iterator to the end of the range to construct.
 *
  * @note Requires that the value type of Iterator is DefaultConstructible.
 */
template<class Iterator>
void
uninitialized_default_construct(Iterator begin, Iterator end)
{
    Iterator current = begin;
    try
    {
        while (current != end)
        {
            pw::construct_at(pw::addressof(*current));
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
#endif /*  INCLUDED_PW_IMPL_UNINITIALIZED_DEFAULT_CONSTRUCT_H */

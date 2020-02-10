#ifndef INCLUDED_PW_IMPL_UNINITIALIZED_FILL_CONSTRUCT_H
#define INCLUDED_PW_IMPL_UNINITIALIZED_FILL_CONSTRUCT_H

#include <pw/impl/addressof.h>
#include <pw/impl/destroy.h>
#include <pw/impl/iterator_traits.h>

namespace pw {

template<class Iterator, class Type>
void
uninitialized_fill(Iterator begin, Iterator end, Type const& value)
{
    Iterator current = begin;
    try
    {
        while (current != end)
        {
            ::new (static_cast<void*>(pw::addressof(*current))) Type(value);
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

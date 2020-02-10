#ifndef INCLUDED_PW_IMPL_UNINITIALIZED_DEFAULT_CONSTRUCT_H
#defineINCLUDED_PW_IMPL_UNINITIALIZED_DEFAULT_CONSTRUCT_H

#include <pw/impl/addressof.h>
#include <pw/impl/destroy.h>
#include <pw/impl/iterator_traits.h>

namespace pw {

template<class Iterator>
void
uninitialized_default_construct(Iterator begin, Iterator end)
{
    using Value      = typename pw::iterator_traits<Iterator>::value_type;
    Iterator current = begin;
    try
    {
        while (current != end)
        {
            ::new (static_cast<void*>(pw::addressof(*current))) Value;
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

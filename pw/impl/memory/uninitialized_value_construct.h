#ifndef INCLUDED_PW_IMPL_UNINITIALIZED_VALUE_CONSTRUCT_H
#define INCLUDED_PW_IMPL_UNINITIALIZED_VALUE_CONSTRUCT_H

#include <pw/impl/memory/addressof.h>
#include <pw/impl/memory/construct_at.h>
#include <pw/impl/memory/destroy.h>

namespace pw {
template<class Iterator>
void
uninitialized_value_construct(Iterator begin, Iterator end)
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
#endif /* INCLUDED_PW_IMPL_UNINITIALIZED_VALUE_CONSTRUCT_H */

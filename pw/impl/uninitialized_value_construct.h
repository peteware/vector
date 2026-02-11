#ifndef INCLUDED_PW_IMPL_UNINITIALIZED_VALUE_CONSTRUCT_H
#define INCLUDED_PW_IMPL_UNINITIALIZED_VALUE_CONSTRUCT_H

#include <pw/impl/addressof.h>
#include <pw/impl/construct_at.h>
#include <pw/impl/destroy.h>

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

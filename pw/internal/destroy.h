#ifndef INCLUDED_PW_INTERNAL_DESTROY_H
#define INCLUDED_PW_INTERNAL_DESTROY_H

#include <pw/impl/addressof.h>
#include <pw/impl/destroy_at.h>

namespace pw { namespace internal {

template<typename Iterator>
void
destroy(Iterator first, Iterator end)
{
    while (first != end)
    {
        pw::destroy_at(pw::addressof(*first++));
    }
}

}} // namespace pw::internal

#endif /* INCLUDED_PW_INTERNAL_DESTROY_H */

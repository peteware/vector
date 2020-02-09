#ifndef INCLUDED_PW_IMPL_DESTROY_H
#define INCLUDED_PW_IMPL_DESTROY_H

#include <pw/impl/addressof.h>
#include <pw/impl/destroy_at.h>

namespace pw {

template<typename Iterator>
void
destroy(Iterator first, Iterator end)
{
    while (first != end)
    {
        pw::destroy_at(pw::addressof(*first++));
    }
}

} // namespace pw

#endif /* INCLUDED_PW_IMPL_DESTROY_H */

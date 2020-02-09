#ifndef INCLUDED_PW_INTERANL_COPYCONSTRUCT_H
#define INCLUDED_PW_INTERANL_COPYCONSTRUCT_H

#include <pw/impl/allocator_traits.h>

namespace pw { namespace internal {

template<class InputIterator, class OutputIterator, class Allocator>
OutputIterator
copyconstruct(Allocator& alloc, InputIterator begin, InputIterator end, OutputIterator out)
{
    while (begin != end)
    {
        allocator_traits<Allocator>::construct(alloc, &(*out++), *begin++);
    }
    return out;
}

}} // namespace pw::internal

#endif /* INCLUDED_PW_INTERANL_COPYCONSTRUCT_H */

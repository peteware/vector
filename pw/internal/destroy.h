#ifndef INCLUDED_PW_INTERNAL_DESTROY_H
#define INCLUDED_PW_INTERNAL_DESTROY_H

namespace pw { namespace internal {

template<typename Allocator, typename Iterator>
void
destroy(Allocator& alloc, Iterator first, Iterator end)
{
    while (first != end)
    {
        allocator_traits<Allocator>::deallocate(alloc, first++);
    }
}

}} // namespace pw::internal

#endif /* INCLUDED_PW_INTERNAL_DESTROY_H */

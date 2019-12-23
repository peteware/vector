#ifndef INCLUDED_PW_INTERNAL_ALLOCATE_H
#define INCLUDED_PW_INTERNAL_ALLOCATE_H

#include <pw/internal/size.h>
#include <pw/internal/ptrdiff.h>
#include <pw/internal/allocator_traits.h>
#include <pw/internal/allocator.h>

#include <pw/utility>

namespace pw { namespace internal {

template<class Type, class Allocator>
struct Allocate
{
    using value_type      = Type;
    using allocator_type  = Allocator;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using pointer         = typename internal::allocator_traits<Allocator>::pointer;
    using const_pointer   = typename internal::allocator_traits<Allocator>::const_pointer;

    pointer               m_begin;
    pointer               m_end;
    size_type             m_allocated;
    allocator_type const& m_alloc;

    Allocate(allocator_type const& alloc)
        : m_begin()
        , m_end()
        , m_allocated(0)
        , m_alloc(alloc)
    {
    }

    Allocate(Allocate const& copy)
        : m_begin(copy.m_begin)
        , m_end(copy.m_end)
        , m_allocated(copy.m_allocated)
        , m_alloc(copy.m_alloc)
    {
    }

    ~Allocate() {}

    size_type size() const { return m_end - m_begin; }

    void swap(Allocate& op2)
    {
        if (m_alloc == op2.m_alloc)
        {
            pw::swap(m_begin, op2.m_begin);
            pw::swap(m_end, op2.m_end);
            pw::swap(m_allocated, op2.m_allocated);
        }
        else
        {
        }
    }

    bool hasroom(size_type count) { return m_allocated - size() < count; }
};

}} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_ALLOCATE_H */

#ifndef INCLUDED_PW_INTERNAL_ALLOCATE_H
#define INCLUDED_PW_INTERNAL_ALLOCATE_H

#include <pw/internal/allocator.h>
#include <pw/internal/allocator_traits.h>
#include <pw/internal/copy.h>
#include <pw/internal/max.h>
#include <pw/internal/ptrdiff.h>
#include <pw/internal/size.h>
#include <pw/internal/swap.h>

#include <stdexcept>

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

    pointer        m_begin;
    pointer        m_end;
    size_type      m_allocated;
    allocator_type m_alloc;

    enum { INITIAL_SIZE = 10 };

    Allocate(allocator_type const& alloc)
        : m_begin(0)
        , m_end(0)
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
            internal::swap(m_begin, op2.m_begin);
            internal::swap(m_end, op2.m_end);
            internal::swap(m_allocated, op2.m_allocated);
        }
        else
        {
            throw(std::runtime_error("Allocate::swap() Not implemented"));
        }
    }

    bool hasroom(size_type count) { return m_allocated - size() > count; }

    void resize(size_type count)
    {
        size_type need;

        if (m_allocated == 0)
        {
            need = internal::max(count, static_cast<size_type>(INITIAL_SIZE));
        }
        else
        {
            need = internal::max(count, 2 * m_allocated);
        }

        pointer p = allocator_traits<Allocator>::allocate(m_alloc, m_allocated + need);
        internal::copy(m_begin, m_end, p);
        m_end       = p + size();
        m_allocated = m_allocated + need;
        m_begin     = p;
    }

    void add(Type const& value) { *m_end++ = value; }
};

}} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_ALLOCATE_H */
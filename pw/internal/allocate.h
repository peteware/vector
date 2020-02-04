#ifndef INCLUDED_PW_INTERNAL_ALLOCATE_H
#define INCLUDED_PW_INTERNAL_ALLOCATE_H

#include <pw/impl/allocator.h>
#include <pw/impl/allocator_traits.h>
#include <pw/impl/copy.h>
#include <pw/impl/ptrdiff.h>
#include <pw/impl/size.h>
#include <pw/impl/swap.h>

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
    using pointer         = typename allocator_traits<Allocator>::pointer;
    using const_pointer   = typename allocator_traits<Allocator>::const_pointer;

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

    ~Allocate()
    {
        allocator_traits<Allocator>::deallocate(m_alloc, m_begin, m_allocated);
    }

    size_type size() const
    {
        return m_end - m_begin;
    }

    void forget()
    {
        m_begin = m_end = 0;
        m_allocated     = 0;
    }

    void swap(Allocate& op2)
    {
        if (m_alloc == op2.m_alloc)
        {
            swap(m_begin, op2.m_begin);
            swap(m_end, op2.m_end);
            swap(m_allocated, op2.m_allocated);
        }
        else
        {
            throw(std::runtime_error("Allocate::swap() Not implemented"));
        }
    }

    bool hasroom(size_type count) const
    {
        return m_allocated - size() > count;
    }

    void expand()
    {
        size_type need;

        if (m_allocated == 0)
        {
            need = INITIAL_SIZE;
        }
        else
        {
            need = 2 * m_allocated;
        }

        allocate(need);
    }

    void resize(size_type count)
    {
        allocate(count);
        m_end = m_begin + m_allocated;
    }

    void allocate(size_type count)
    {
        pointer p = allocator_traits<Allocator>::allocate(m_alloc, count);
        copy(m_begin, m_end, p);
        m_end       = p + size();
        m_allocated = count;
        m_begin     = p;
    }

    void add(Type const& value)
    {
        allocator_traits<Allocator>::construct(m_alloc, m_end++, value);
    }
};

}} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_ALLOCATE_H */

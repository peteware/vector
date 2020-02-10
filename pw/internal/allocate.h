#ifndef INCLUDED_PW_INTERNAL_ALLOCATE_H
#define INCLUDED_PW_INTERNAL_ALLOCATE_H

#include <pw/impl/allocator.h>
#include <pw/impl/allocator_traits.h>
#include <pw/impl/copy.h>
#include <pw/impl/destroy.h>
#include <pw/impl/move_alg.h>
#include <pw/impl/ptrdiff.h>
#include <pw/impl/size.h>
#include <pw/impl/swap.h>
#include <pw/impl/uninitialized_copy.h>
#include <pw/impl/uninitialized_move.h>

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
        : m_begin(0)
        , m_end(0)
        , m_allocated(0)
        , m_alloc(copy.m_alloc)
    {
        allocate(copy.size());
        pw::uninitialized_copy(copy.m_begin, copy.m_end, m_begin);
        setsize(copy.size());
    }

    ~Allocate()
    {
        allocator_traits<Allocator>::deallocate(m_alloc, m_begin, m_allocated);
    }

    size_type size() const
    {
        return m_end - m_begin;
    }

    size_type capacity() const
    {
        return m_allocated;
    }

    void clear()
    {
        allocator_traits<Allocator>::deallocate(m_alloc, m_begin, m_allocated);
        m_begin     = 0;
        m_end       = 0;
        m_allocated = 0;
    }

    void forget()
    {
        m_begin     = 0;
        m_end       = 0;
        m_allocated = 0;
    }

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
            throw(std::runtime_error("Allocate::swap() Not implemented"));
        }
    }

    bool hasroom(size_type count) const
    {
        return m_allocated - size() >= count;
    }

    Allocate& expand()
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
        return *this;
    }

    Allocate& setsize(size_type count)
    {
        m_end = m_begin + count;
        return *this;
    }

    Allocate& allocate(size_type count)
    {
        pointer p = allocator_traits<Allocator>::allocate(m_alloc, count);
        pw::uninitialized_move(m_begin, m_end, p);
        pw::destroy(m_begin, m_end);
        // TODO: Allocate::allocate() unit tests didn't detect memory not deallocated
        allocator_traits<Allocator>::deallocate(m_alloc, m_begin, m_allocated);
        m_end       = p + size();
        m_begin     = p;
        m_allocated = count;
        return *this;
    }

    Allocate& add(Type const& value)
    {
        allocator_traits<Allocator>::construct(m_alloc, m_end++, value);
        return *this;
    }
};

}} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_ALLOCATE_H */

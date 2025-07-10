#ifndef INCLUDED_PW_INTERNAL_STORAGE_H
#define INCLUDED_PW_INTERNAL_STORAGE_H

#include <pw/impl/allocator.h>
#include <pw/impl/allocator_traits.h>
#include <pw/impl/copy.h>
#include <pw/impl/destroy.h>
#include <pw/impl/distance.h>
#include <pw/impl/exchange.h>
#include <pw/impl/max.h>
#include <pw/impl/min.h>
#include <pw/impl/move_alg.h>
#include <pw/impl/next.h>
#include <pw/impl/ptrdiff.h>
#include <pw/impl/size.h>
#include <pw/impl/swap.h>
#include <pw/impl/uninitialized_copy.h>
#include <pw/impl/uninitialized_fill.h>
#include <pw/impl/uninitialized_move.h>

#include <iostream>
#include <stdexcept>

namespace pw::internal {

/**
 * Storage managed contiguous memory of Type objects.
 *
 * The total memory is allocated when the Storage
 * object is created.  To change the amount of memory
 * use move() to create a new Storage object with the
 * contents move()'d into the new Storage object.
 *
 * The region between m_begin and m_end represents
 * initialized Type objects:
 *
 * @example
 *    ┌────┬────┬────┬────┬────┬────┬────┐
 * ┌─▶│ 1  │ 2  │ 3  │ 4  │◀───┼─┐  │    │
 * │  └────┴────┴────┴────┴────┴─┼──┴────┘
 * │                             │        
 * │        m_allocated = 7      │        
 * │                             │        
 * └─── m_begin         m_end  ──┘        
 * @endexample
 * 
 */
template<class Type, class Allocator = pw::allocator<Type>>
struct Storage
{
    using value_type      = Type;
    using allocator_type  = Allocator;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using pointer         = typename allocator_traits<Allocator>::pointer;
    using const_pointer   = typename allocator_traits<Allocator>::const_pointer;
    using iterator        = pointer;
    using const_iterator  = const_pointer;

private:
    allocator_type m_alloc;
    pointer        m_begin;
    pointer        m_end;
    size_type      m_allocated;

    enum { INITIAL_SIZE = 1 };

public:
    Storage(allocator_type const& alloc = allocator_type());
    explicit Storage(size_type count, allocator_type const& alloc = allocator_type());
    ~Storage();
    Storage&              operator=(Storage& op2) = delete;

    iterator              begin();
    iterator              end();
    const_iterator        begin() const;
    const_iterator        end() const;
    Storage&              uninitialized_fill(iterator first, iterator last, value_type const& val);
    Storage&              set_size(size_type count);
    size_type             size() const;
    size_type             capacity() const;
    size_type             newsize() const;
    bool                  hascapacity(size_type count = 1) const;
    bool                  empty() const;
    void                  push_back(value_type const& value);
    void                  push_back(value_type&& value);
    allocator_type&       get_allocator();
    allocator_type const& get_allocator() const;
    void                  swap(Storage& op2)
    {
        pw::swap(m_begin, op2.m_begin);
        pw::swap(m_end, op2.m_end);
        pw::swap(m_allocated, op2.m_allocated);
    }
    friend void swap(Storage& op1, Storage& op2)
    {
        pw::swap(op1.m_begin, op2.m_begin);
        pw::swap(op1.m_end, op2.m_end);
        pw::swap(op1.m_allocated, op2.m_allocated);
    }
    void moveto(iterator begin, iterator end, iterator dest);
    template<class Iterator>
    void copyto(Iterator first, Iterator last, iterator dest);

private:
};

template<class Type, class Allocator>
Storage<Type, Allocator>::Storage(allocator_type const& alloc)
    : m_alloc(alloc)
    , m_begin(0)
    , m_end(0)
    , m_allocated(0)
{
}

template<class Type, class Allocator>
Storage<Type, Allocator>::Storage(size_type count, allocator_type const& alloc)
    : m_alloc(alloc)
    , m_begin(allocator_traits<Allocator>::allocate(m_alloc, count))
    , m_end(m_begin)
    , m_allocated(count)
{
}

template<class Type, class Allocator>
Storage<Type, Allocator>::~Storage()
{
    pw::destroy(begin(), end());
    allocator_traits<Allocator>::deallocate(m_alloc, m_begin, m_allocated);
}

template<class Type, class Allocator>
typename Storage<Type, Allocator>::iterator
Storage<Type, Allocator>::begin()
{
    return m_begin;
}

template<class Type, class Allocator>
typename Storage<Type, Allocator>::iterator
Storage<Type, Allocator>::end()
{
    return m_end;
}

template<class Type, class Allocator>
typename Storage<Type, Allocator>::const_iterator
Storage<Type, Allocator>::begin() const
{
    return m_begin;
}

template<class Type, class Allocator>
typename Storage<Type, Allocator>::const_iterator
Storage<Type, Allocator>::end() const
{
    return m_end;
}

template<class Type, class Allocator>
Storage<Type, Allocator>&
Storage<Type, Allocator>::set_size(size_type count)
{
    m_end = m_begin + count;
    return *this;
}

template<class Type, class Allocator>
typename Storage<Type, Allocator>::size_type
Storage<Type, Allocator>::size() const
{
    return m_end - m_begin;
}

template<class Type, class Allocator>
typename Storage<Type, Allocator>::size_type
Storage<Type, Allocator>::capacity() const
{
    return m_allocated;
}

template<class Type, class Allocator>
typename Storage<Type, Allocator>::size_type
Storage<Type, Allocator>::newsize() const
{
    return pw::max((size_type)INITIAL_SIZE, m_allocated * 2);
}

template<class Type, class Allocator>
bool
Storage<Type, Allocator>::hascapacity(size_type count) const
{
    return count <= capacity() - size();
}

template<class Type, class Allocator>
bool
Storage<Type, Allocator>::empty() const
{
    return m_begin == m_end;
}

template<class Type, class Allocator>
void
Storage<Type, Allocator>::push_back(value_type const& value)
{
    allocator_traits<Allocator>::construct(m_alloc, m_end++, value);
}

template<class Type, class Allocator>
void
Storage<Type, Allocator>::push_back(value_type&& value)
{
    allocator_traits<Allocator>::construct(m_alloc, m_end++, pw::move(value));
}

template<class Type, class Allocator>
Allocator&
Storage<Type, Allocator>::get_allocator()
{
    return m_alloc;
}

template<class Type, class Allocator>
Allocator const&
Storage<Type, Allocator>::get_allocator() const
{
    return m_alloc;
}

template<class Type, class Allocator>
void
Storage<Type, Allocator>::moveto(iterator begin, iterator end, iterator dest)
{
    while (end != begin)
    {
        --dest;
        --end;
        if (dest >= m_end)
        {
            allocator_traits<Allocator>::construct(m_alloc, dest, pw::move(*end));
        }
        else
        {
            *dest = pw::move(*end);
        }
    }
}

template<class Type, class Allocator>
template<class Iterator>
void
Storage<Type, Allocator>::copyto(Iterator first, Iterator last, iterator dest)
{
    auto overlap            = pw::distance(dest, end());
    auto uninitialized_dest = pw::copy(first, next(first, overlap), dest);

    pw::uninitialized_copy(next(first, overlap), last, uninitialized_dest);
}

} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_STORAGE_H */

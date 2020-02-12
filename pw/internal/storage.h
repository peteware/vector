#ifndef INCLUDED_PW_INTERNAL_STORAGE_H
#define INCLUDED_PW_INTERNAL_STORAGE_H

#include <pw/impl/allocator.h>
#include <pw/impl/allocator_traits.h>
#include <pw/impl/copy.h>
#include <pw/impl/destroy.h>
#include <pw/impl/min.h>
#include <pw/impl/move_alg.h>
#include <pw/impl/ptrdiff.h>
#include <pw/impl/size.h>
#include <pw/impl/swap.h>
#include <pw/impl/uninitialized_copy.h>
#include <pw/impl/uninitialized_move.h>

#include <stdexcept>

namespace pw { namespace internal {

/**
 * ┌■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■┬─────────────────┐
 * └■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■┼─────────────────┘
 * │                              │                  
 * │                              │                  
 * ▼┌───────┐                     ▼┌──────┐          
 *  │m_begin│                      │m_end │          
 *  └───────┘                      └──────┘          
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

    allocator_type m_alloc;
    pointer        m_begin;
    pointer        m_end;
    size_type      m_allocated;

    enum { INITIAL_SIZE = 10 };

    Storage(allocator_type const& alloc = allocator_type());
    Storage(size_type count, allocator_type const& alloc = allocator_type());
    Storage(Storage const& copy, allocator_type const& alloc = allocator_type());
    Storage(Storage&& copy, allocator_type const& alloc = allocator_type());
    ~Storage();
    /**
     * Allocate enough space for count records.  Then up to
     * count records are moved into new Storage.
     */
    Storage   move(size_type count, allocator_type const& alloc = allocator_type());
    iterator  begin();
    iterator  end();
    size_type size() const;
    size_type capacity() const;
    void      push_back(value_type const& value);
    void      push_back(value_type&& value);
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
Storage<Type, Allocator>::Storage(Storage const& copy, allocator_type const& alloc)
    : m_alloc(alloc)
    , m_begin(allocator_traits<Allocator>::allocate(m_alloc, copy.size()))
    , m_end(m_begin + copy.size())
    , m_allocated(copy.size())
{
    pw::uninitialized_copy(copy.m_begin, copy.m_end, m_begin);
}

template<class Type, class Allocator>
Storage<Type, Allocator>::Storage(Storage&& copy, allocator_type const& alloc)
    : m_alloc(alloc)
    , m_begin(0)
    , m_end(0)
    , m_allocated(0)
{
    using pw::swap;

    swap(m_begin, copy.m_begin);
    swap(m_end, copy.m_end);
    swap(m_allocated, copy.m_allocated);
}

template<class Type, class Allocator>
Storage<Type, Allocator>::~Storage()
{
    pw::destroy(begin(), end());
    allocator_traits<Allocator>::deallocate(m_alloc, m_begin, m_allocated);
}

/**
 * Allocate enough space for count records.  Then up to
 * count records are moved into new Storage and others
 * are destroyed.
 */
template<class Type, class Allocator>
Storage<Type, Allocator>
Storage<Type, Allocator>::move(size_type count, allocator_type const& alloc)
{
    Storage   s(count, alloc);
    size_type final = min(count, size());

    pw::uninitialized_move(begin(), begin() + final, s.begin());
    pw::destroy(begin(), end());
    m_end   = m_begin;
    s.m_end = s.m_begin + final;
    return s;
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
void
Storage<Type, Allocator>::push_back(value_type const& value)
{
    *m_end++ = value;
}

template<class Type, class Allocator>
void
Storage<Type, Allocator>::push_back(value_type&& value)
{
}

}} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_STORAGE_H */

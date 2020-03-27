#ifndef INCLUDED_PW_INTERNAL_STORAGE_H
#define INCLUDED_PW_INTERNAL_STORAGE_H

#include <pw/impl/allocator.h>
#include <pw/impl/allocator_traits.h>
#include <pw/impl/copy.h>
#include <pw/impl/destroy.h>
#include <pw/impl/max.h>
#include <pw/impl/min.h>
#include <pw/impl/move_alg.h>
#include <pw/impl/ptrdiff.h>
#include <pw/impl/size.h>
#include <pw/impl/swap.h>
#include <pw/impl/uninitialized_copy.h>
#include <pw/impl/uninitialized_fill.h>
#include <pw/impl/uninitialized_move.h>

#include <stdexcept>

namespace pw { namespace internal {

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

    enum { INITIAL_SIZE = 10 };

public:
    Storage(allocator_type const& alloc = allocator_type());
    Storage(size_type count, allocator_type const& alloc = allocator_type());
    Storage(Storage const& copy);
    Storage(Storage const& copy, allocator_type const& alloc);
    Storage(Storage&& copy);
    Storage(Storage&& copy, size_type count);
    ~Storage();
    Storage& operator=(Storage op2);
    /**
     * Allocate enough space for count records.  Then up to
     * count records are moved into new Storage.
     */
    Storage        move(size_type count);
    void           move(size_type offset, size_type count, Type const& value);
    Storage        resize(size_type offset, size_type count, Type const& value);
    iterator       begin();
    iterator       end();
    const_iterator begin() const;
    const_iterator end() const;
    Storage&       set_size(size_type count);
    size_type      size() const;
    size_type      capacity() const;
    size_type      newsize() const;
    bool           hascapacity(size_type count = 1) const;
    bool           empty() const;
    void           push_back(value_type const& value);
    void           push_back(value_type&& value);
    allocator_type get_allocator() const;
    friend void    swap(Storage& op1, Storage& op2)
    {
        pw::swap(op1.m_begin, op2.m_begin);
        pw::swap(op1.m_end, op2.m_end);
        pw::swap(op1.m_allocated, op2.m_allocated);
    }

private:
    void moveto(iterator begin, iterator end, iterator dest);
    void cons(iterator begin, iterator end, Type const& value);
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
Storage<Type, Allocator>::Storage(Storage const& copy)
    : m_alloc(copy.m_alloc)
    , m_begin(allocator_traits<Allocator>::allocate(m_alloc, copy.size()))
    , m_end(m_begin + copy.size())
    , m_allocated(copy.size())
{
    pw::uninitialized_copy(copy.m_begin, copy.m_end, m_begin);
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
Storage<Type, Allocator>::Storage(Storage&& copy)
    : m_alloc(copy.m_alloc)
    , m_begin(0)
    , m_end(0)
    , m_allocated(0)
{
    swap(*this, copy);
}

template<class Type, class Allocator>
Storage<Type, Allocator>::~Storage()
{
    pw::destroy(begin(), end());
    allocator_traits<Allocator>::deallocate(m_alloc, m_begin, m_allocated);
}

template<class Type, class Allocator>
Storage<Type, Allocator>&
Storage<Type, Allocator>::operator=(Storage op2)
{
    swap(*this, op2);
    return *this;
}

/**
 * Allocate enough space for count records and
 * move/copy them
 */
template<class Type, class Allocator>
Storage<Type, Allocator>
Storage<Type, Allocator>::move(size_type count)
{
    Storage   s(count, m_alloc);
    size_type final = min(count, size());

    pw::uninitialized_move(begin(), begin() + final, s.begin());
    s.set_size(final);
    return s;
}

template<class Type, class Allocator>
Storage<Type, Allocator>
Storage<Type, Allocator>::resize(size_type offset, size_type count, Type const& value)
{
    Storage s(size() + count, m_alloc);

    pw::uninitialized_move(begin(), begin() + offset, s.begin());
    pw::uninitialized_move(begin() + offset, end(), s.begin() + offset + count);
    pw::uninitialized_fill(s.begin() + offset, s.begin() + offset + count, value);
    s.set_size(size() + count);
    return s;
}

/**
 * Allocate enough space for count records.  Then up to
 * count records are moved into new Storage and others
 * are destroyed.
 */
template<class Type, class Allocator>
void
Storage<Type, Allocator>::move(size_type offset, size_type count, Type const& value)
{
    moveto(m_begin + offset, m_end, m_end + offset);
    cons(m_begin + offset, m_begin + offset + count, value);
    set_size(size() + count);
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
    return pw::max((size_type)8, m_allocated * 2);
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
Allocator
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
void
Storage<Type, Allocator>::cons(iterator begin, iterator end, Type const& value)
{
    while (begin != end)
    {
        if (begin < end)
        {
            *begin = value;
        }
        else
        {
            allocator_traits<Allocator>::construct(m_alloc, begin, value);
        }
        ++begin;
    }
}

}} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_STORAGE_H */

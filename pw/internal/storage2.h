#ifndef INCLUDED_PW_INTERNAL_STORAGE2_H
#define INCLUDED_PW_INTERNAL_STORAGE2_H
#endif /* INCLUDED_PW_INTERNAL_STORAGE2_H */

#include <pw/impl/allocator.h>
#include <pw/impl/allocator_traits.h>
#include <pw/impl/destroy.h>
#include <pw/impl/size.h>
#include <pw/impl/swap.h>
#include <pw/impl/uninitialized_move.h>

namespace pw { namespace internal {
template<class Type, class Allocator = pw::allocator<Type>>
struct Storage2
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

    Storage2(allocator_type const& alloc);
    ~Storage2();

    constexpr void reserve(size_type count);
    constexpr bool empty() const;

    constexpr pointer begin();
    constexpr pointer end();

    Storage2& set_size(size_type size);

private:
    allocator_type m_alloc;
    pointer        m_begin;
    size_type      m_size;
    size_type      m_allocated;

    enum { INITIAL_SIZE = 1 };
};

template<class Type, class Allocator>
Storage2<Type, Allocator>::Storage2(allocator_type const& alloc)
    : m_alloc(alloc)
    , m_begin(0)
    , m_size(0)
    , m_allocated(0)
{
}

template<class Type, class Allocator>
Storage2<Type, Allocator>::~Storage2()
{
    pw::destroy(m_begin, m_begin + m_size);
    allocator_traits<Allocator>::deallocate(m_alloc, m_begin, m_allocated);
}

template<class Type, class Allocator>
constexpr void
Storage2<Type, Allocator>::reserve(size_type count)
{
    pointer   tmp = allocator_traits<Allocator>::allocate(m_alloc, count);
    size_type old;

    pw::uninitialized_move(begin(), end(), tmp);
    pw::destroy(begin(), end());
    allocator_traits<Allocator>::deallocate(m_alloc, m_begin, m_allocated);
    swap(tmp, m_begin);
    swap(count, m_allocated);
}

template<class Type, class Allocator>
constexpr bool
Storage2<Type, Allocator>::empty() const
{
    return m_size == 0;
}

template<class Type, class Allocator>
constexpr typename Storage2<Type, Allocator>::pointer
Storage2<Type, Allocator>::begin()
{
    return m_begin;
}

template<class Type, class Allocator>
constexpr typename Storage2<Type, Allocator>::pointer
Storage2<Type, Allocator>::end()
{
    return m_begin + m_size;
}

template<class Type, class Allocator>
Storage2<Type, Allocator>&
Storage2<Type, Allocator>::set_size(size_type size)
{
    m_size = size;
    return *this;
}

}} // namespace pw::internal

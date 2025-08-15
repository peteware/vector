#ifndef INCLUDED_PW_INTERNAL_STORAGE2_H
#define INCLUDED_PW_INTERNAL_STORAGE2_H

#include <functional>
#include <pw/impl/addressof.h>
#include <pw/impl/allocator.h>
#include <pw/impl/allocator_traits.h>
#include <pw/impl/destroy.h>
#include <pw/impl/max.h>
#include <pw/impl/move.h>
#include <pw/impl/swap.h>
#include <pw/impl/uninitialized_move.h>

namespace pw::internal {

/**
 * The Storage2 struct is an internal implementation that
 * helps insure any allocated memory is deallocated.
 *
 * @verbatim
 *    ┌────┬────┬────┬────┬────┬────┬────┐
 * ┌─▶│ 1  │ 2  │ 3  │ 4  │    │    │    │
 * │  └────┴────┴────┴────┴────┴────┴────┘
 * │    m_size = 4        ▲
 * │                      │
 * │    m_allocated = 7   │
 * │                   end()
 * └─── m_begin
 * @endverbatim
 */

template<class Type, class Allocator = pw::allocator<Type>>
struct Storage2
{
    using value_type      = Type;
    using allocator_type  = Allocator;
    using size_type       = allocator_traits<Allocator>::size_type;
    using difference_type = allocator_traits<Allocator>::difference_type;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using pointer         = allocator_traits<Allocator>::pointer;
    using const_pointer   = allocator_traits<Allocator>::const_pointer;
    using iterator        = pointer;
    using const_iterator  = const_pointer;

    constexpr explicit Storage2(allocator_type const& alloc);
    constexpr Storage2(allocator_type const& alloc, size_type count);
    constexpr ~Storage2();

    constexpr void                    reset();
    [[nodiscard]] constexpr bool      empty() const noexcept;
    constexpr pointer                 begin() noexcept;
    constexpr const_pointer           begin() const noexcept;
    constexpr pointer                 end() noexcept;
    constexpr const_pointer           end() const noexcept;
    constexpr Storage2&               set_size(size_type size) noexcept;
    constexpr size_type               size() const noexcept;
    constexpr size_type               allocated() const noexcept;
    [[nodiscard]] constexpr size_type calc_size() const noexcept;
    constexpr allocator_type&         allocator() noexcept;
    constexpr allocator_type          copy_allocator() const;
    constexpr void                    swap(Storage2& other, bool swap_allocator)
        noexcept(allocator_traits<allocator_type>::propagate_on_container_swap::value ||
                 allocator_traits<allocator_type>::is_always_equal::value);

    constexpr void                    uninitialized_fill(iterator begin, iterator end, value_type const& value);
    constexpr void                    uninitialized_copy(const_iterator src_begin, const_iterator src_end, iterator dest);
    constexpr void                    uninitialized_default_construct(iterator begin, iterator end);
    constexpr iterator                uninitialized_move(iterator src_begin, iterator src_end, iterator dest);
    constexpr iterator                copy(const_iterator src_begin, const_iterator src_end, iterator dest);

private:
    allocator_type m_alloc;
    pointer        m_begin;
    size_type      m_size;
    size_type      m_allocated;

    enum { INITIAL_SIZE = 1 };
};

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::Storage2(allocator_type const& alloc)
    : m_alloc(alloc)
    , m_begin(nullptr)
    , m_size(0)
    , m_allocated(0)
{
}

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::Storage2(allocator_type const& alloc, size_type count)
    : m_alloc(alloc)
    , m_begin(allocator_traits<Allocator>::allocate(m_alloc, count))
    , m_size(0)
    , m_allocated(count)
{
}

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::~Storage2()
{
    pw::destroy(m_begin, m_begin + m_size);
    allocator_traits<Allocator>::deallocate(m_alloc, m_begin, m_allocated);
}

template<class Type, class Allocator>
constexpr void
Storage2<Type, Allocator>::reset()
{
    if (m_begin)
    {
        pw::destroy(m_begin, m_begin + m_size);
        allocator_traits<Allocator>::deallocate(m_alloc, m_begin, m_allocated);
    }
    m_begin     = nullptr;
    m_size      = 0;
    m_allocated = 0;
}
template<class Type, class Allocator>
constexpr bool
Storage2<Type, Allocator>::empty() const noexcept
{
    return m_size == 0;
}

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::pointer
Storage2<Type, Allocator>::begin() noexcept
{
    return m_begin;
}

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::const_pointer
Storage2<Type, Allocator>::begin() const noexcept
{
    return m_begin;
}

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::pointer
Storage2<Type, Allocator>::end() noexcept
{
    return m_begin + m_size;
}

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::const_pointer
Storage2<Type, Allocator>::end() const noexcept
{
    return m_begin + m_size;
}

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>&
Storage2<Type, Allocator>::set_size(size_type size) noexcept
{
    m_size = size;
    return *this;
}

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::size_type
Storage2<Type, Allocator>::size() const noexcept
{
    return m_size;
}

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::size_type
Storage2<Type, Allocator>::allocated() const noexcept
{
    return m_allocated;
}

/**
 * Doubles the existing value for allocated()
 *
 * @return The next size to allocate (always >= 1).
 */
template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::size_type
Storage2<Type, Allocator>::calc_size() const noexcept
{
    return max(static_cast<size_type>(1), 2 * m_allocated);
}

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::allocator_type
Storage2<Type, Allocator>::copy_allocator() const
{
    return m_alloc;
}

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::allocator_type&
Storage2<Type, Allocator>::allocator() noexcept
{
    return m_alloc;
}

template<class Type, class Allocator>
constexpr void
Storage2<Type, Allocator>::swap(Storage2& other, bool swap_allocator)
    noexcept(pw::allocator_traits<allocator_type>::propagate_on_container_swap::value ||
             pw::allocator_traits<allocator_type>::is_always_equal::value)
{
    using pw::swap;
    if (swap_allocator)
    {
        pw::swap(m_alloc, other.m_alloc);
    }
    pw::swap(m_begin, other.m_begin);
    pw::swap(m_size, other.m_size);
    pw::swap(m_allocated, other.m_allocated);
}

template<class Type, class Allocator>
constexpr void
Storage2<Type, Allocator>::uninitialized_fill(iterator begin, iterator end, value_type const& value)
{
    iterator current = begin;
    try
    {
        while (current != end)
        {
            allocator_traits<Allocator>::construct(m_alloc, pw::addressof(*current), value);
            ++current;
        }
    }
    catch (...)
    {
        while (begin != current)
        {
            allocator_traits<Allocator>::destroy(m_alloc, pw::addressof(*begin));
            ++begin;
        }
        throw;
    }
}

template<class Type, class Allocator>
constexpr void
Storage2<Type, Allocator>::uninitialized_copy(const_iterator src_begin, const_iterator src_end, iterator dest)
{
    iterator current = dest;
    try
    {
        while (src_begin != src_end)
        {
            allocator_traits<Allocator>::construct(m_alloc, pw::addressof(*current), *src_begin);
            ++current;
            ++src_begin;
        }
    }
    catch (...)
    {
        while (dest != current)
        {
            allocator_traits<Allocator>::destroy(m_alloc, pw::addressof(*dest));
            ++dest;
        }
        throw;
    }
}

template<class Type, class Allocator>
constexpr void
Storage2<Type, Allocator>::uninitialized_default_construct(iterator begin, iterator end)
{
    iterator current = begin;
    try
    {
        while (current != end)
        {
            allocator_traits<Allocator>::construct(m_alloc, pw::addressof(*current));
            ++current;
        }
    }
    catch (...)
    {
        while (begin != current)
        {
            allocator_traits<Allocator>::destroy(m_alloc, pw::addressof(*begin));
            ++begin;
        }
        throw;
    }
}

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::iterator
Storage2<Type, Allocator>::uninitialized_move(iterator src_begin, iterator src_end, iterator dest)
{
    iterator current = dest;
    try
    {
        while (src_begin != src_end)
        {
            allocator_traits<Allocator>::construct(m_alloc, pw::addressof(*current), pw::move(*src_begin));
            ++current;
            ++src_begin;
        }
    }
    catch (...)
    {
        while (dest != current)
        {
            allocator_traits<Allocator>::destroy(m_alloc, pw::addressof(*dest));
            ++dest;
        }
        throw;
    }
    return current;
}

template<class Type, class Allocator>
constexpr Storage2<Type, Allocator>::iterator
Storage2<Type, Allocator>::copy(const_iterator src_begin, const_iterator src_end, iterator dest)
{
    while (src_begin != src_end)
    {
        *dest = *src_begin;
        ++dest;
        ++src_begin;
    }
    return dest;
}
} // namespace pw::internal
#endif /* INCLUDED_PW_INTERNAL_STORAGE2_H */

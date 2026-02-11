#ifndef INCLUDED_PW_INTERNAL_STORAGE_H
#define INCLUDED_PW_INTERNAL_STORAGE_H

#include <pw/impl/algorithm/max.h>
#include <pw/impl/allocator/allocator.h>
#include <pw/impl/allocator_traits.h>
#include <pw/impl/memory/addressof.h>
#include <pw/impl/memory/destroy.h>
#include <pw/impl/memory/uninitialized_construct_using_allocator.h>
#include <pw/impl/utility/move.h>
#include <pw/impl/utility/swap.h>

#include <functional>

namespace pw::internal {

/**
 * The Storage struct is an internal implementation that
 * helps insure any capacity memory is deallocated.
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

template<class Type, class Allocator = allocator<Type>>
struct Storage
{
    using value_type                   = Type;
    using allocator_type               = Allocator;
    using size_type                    = allocator_traits<Allocator>::size_type;
    using difference_type              = allocator_traits<Allocator>::difference_type;
    using reference                    = value_type&;
    using const_reference              = value_type const&;
    using pointer                      = allocator_traits<Allocator>::pointer;
    using const_pointer                = allocator_traits<Allocator>::const_pointer;
    using iterator                     = pointer;
    using const_iterator               = const_pointer;

    Storage()                          = delete; // Require allocator to be provided
    Storage(Storage const&)            = delete; // Not intended to be copied
    Storage(Storage&&)                 = delete; // Not intended to be moved
    Storage& operator=(Storage const&) = delete;
    Storage& operator=(Storage&&)      = delete;
    constexpr explicit Storage(allocator_type const& alloc);
    constexpr Storage(allocator_type const& alloc, size_type count);
    constexpr ~Storage();

    constexpr void                    reset();
    [[nodiscard]] constexpr bool      empty() const noexcept;
    constexpr pointer                 begin() noexcept;
    constexpr const_pointer           begin() const noexcept;
    constexpr pointer                 end() noexcept;
    constexpr const_pointer           end() const noexcept;
    constexpr pointer                 capacity_begin() noexcept;
    constexpr pointer                 capacity_end() noexcept;
    constexpr Storage&                set_size(size_type size) noexcept;
    constexpr size_type               size() const noexcept;
    [[nodiscard]] constexpr size_type calc_size() const noexcept;
    [[nodiscard]] constexpr size_type max_size() const noexcept;
    constexpr size_type               capacity() const noexcept;
    constexpr allocator_type&         allocator() noexcept;
    constexpr allocator_type          copy_allocator() const;
    constexpr void                    swap_allocator(Storage& other);
    constexpr void                    destroy(iterator begin, iterator end);
    constexpr Storage&                reserve(size_type count);
    constexpr iterator                copy(const_iterator begin, const_iterator end, iterator dest);
    constexpr iterator                move(iterator begin, iterator end, iterator dest);
    constexpr iterator                move_backward(iterator begin, iterator end, iterator dest);
    constexpr iterator                fill_n(iterator dest, size_type count, value_type const& value);
    constexpr Storage&                uninitialized_fill(iterator begin, iterator end, value_type const& val);
    constexpr Storage&                uninitialized_default_construct(iterator begin, iterator end);
    constexpr Storage&                uninitialized_move(iterator begin, iterator end, iterator dest);
    constexpr void
    swap(Storage& other) noexcept(allocator_traits<allocator_type>::propagate_on_container_swap::value ||
                                  allocator_traits<allocator_type>::is_always_equal::value);
    template<class... Args>
    constexpr void construct(iterator where, Args&&... args);
    template<class InputIterator>
    constexpr Storage& uninitialized_copy(InputIterator begin, InputIterator end, iterator dest);

private:
    allocator_type m_alloc;
    pointer        m_begin;
    size_type      m_size;
    size_type      m_allocated;

    enum { INITIAL_SIZE = 1 };
};

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::Storage(allocator_type const& alloc)
    : m_alloc(alloc)
    , m_begin(nullptr)
    , m_size(0)
    , m_allocated(0)
{
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::Storage(allocator_type const& alloc, size_type count)
    : m_alloc(alloc)
    , m_begin(allocator_traits<Allocator>::allocate(m_alloc, count))
    , m_size(0)
    , m_allocated(count)
{
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::~Storage()
{
    pw::destroy(m_begin, m_begin + m_size);
    allocator_traits<Allocator>::deallocate(m_alloc, m_begin, m_allocated);
}

template<class Type, class Allocator>
constexpr void
Storage<Type, Allocator>::reset()
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
Storage<Type, Allocator>::empty() const noexcept
{
    return m_size == 0;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::pointer
Storage<Type, Allocator>::begin() noexcept
{
    return m_begin;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::const_pointer
Storage<Type, Allocator>::begin() const noexcept
{
    return m_begin;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::pointer
Storage<Type, Allocator>::end() noexcept
{
    return m_begin + m_size;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::const_pointer
Storage<Type, Allocator>::end() const noexcept
{
    return m_begin + m_size;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::pointer
Storage<Type, Allocator>::capacity_begin() noexcept
{
    return m_begin + m_size;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::pointer
Storage<Type, Allocator>::capacity_end() noexcept
{
    return m_begin + m_allocated;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>&
Storage<Type, Allocator>::set_size(size_type size) noexcept
{
    m_size = size;
    return *this;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::size_type
Storage<Type, Allocator>::size() const noexcept
{
    return m_size;
}

/**
 * Doubles the existing value for capacity()
 *
 * @return The next size to allocate (always >= 1).
 */
template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::size_type
Storage<Type, Allocator>::calc_size() const noexcept
{
    return max(static_cast<size_type>(1), 2 * m_allocated);
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::size_type
Storage<Type, Allocator>::max_size() const noexcept
{
    return allocator_traits<allocator_type>::max_size(m_alloc);
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::size_type
Storage<Type, Allocator>::capacity() const noexcept
{
    return m_allocated;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::allocator_type&
Storage<Type, Allocator>::allocator() noexcept
{
    return m_alloc;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::allocator_type
Storage<Type, Allocator>::copy_allocator() const
{
    return m_alloc;
}
template<class Type, class Allocator>
constexpr void
Storage<Type, Allocator>::swap_allocator(Storage& other)
{
    pw::swap(m_alloc, other.m_alloc);
}

template<class Type, class Allocator>
constexpr void
Storage<Type, Allocator>::swap(Storage& other)
    noexcept(allocator_traits<allocator_type>::propagate_on_container_swap::value ||
             allocator_traits<allocator_type>::is_always_equal::value)
{
    using pw::swap;
    pw::swap(m_begin, other.m_begin);
    pw::swap(m_size, other.m_size);
    pw::swap(m_allocated, other.m_allocated);
}

template<class Type, class Allocator>
template<class... Args>
constexpr void
Storage<Type, Allocator>::construct(iterator where, Args&&... args)
{
    pw::uninitialized_construct_using_allocator(pw::addressof(*where), m_alloc, std::forward<Args>(args)...);
    //allocator_traits<Allocator>::construct(m_alloc, pw::addressof(*where), pw::forward<Args>(args)...);
}

template<class Type, class Allocator>
constexpr void
Storage<Type, Allocator>::destroy(iterator begin, iterator end)
{
    while (begin != end)
    {
        allocator_traits<Allocator>::destroy(m_alloc, pw::addressof(*begin));
        ++begin;
    }
}

/**
 * Reserves memory for at least count elements.
 * Invalidates all iterators and references.
 *
 * @param count The number of elements to reserve space for
 * @return Reference to this storage
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr Storage<Type, Allocator>&
Storage<Type, Allocator>::reserve(size_type count)
{
    pointer p = allocator_traits<Allocator>::allocate(m_alloc, count);
    if (m_begin)
    {
        destroy(begin(), end());
        allocator_traits<Allocator>::deallocate(m_alloc, begin(), m_allocated);
    }
    m_begin     = p;
    m_size      = 0;
    m_allocated = count;
    return *this;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::iterator
// ReSharper disable once CppMemberFunctionMayBeStatic
Storage<Type, Allocator>::copy(const_iterator begin, const_iterator end, iterator dest)
{
    while (begin != end)
    {
        *dest = *begin;
        ++dest;
        ++begin;
    }
    return dest;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::iterator
Storage<Type, Allocator>::move(iterator begin, iterator end, iterator dest)
{
    while (begin != end)
    {
        *dest++ = pw::move(*begin++);
    }
    return dest;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::iterator
Storage<Type, Allocator>::move_backward(iterator begin, iterator end, iterator dest)
{
    while (begin != end)
    {
        --dest;
        --end;
        construct(dest, pw::move(*end));
    }
    return dest;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>::iterator
Storage<Type, Allocator>::fill_n(iterator dest, size_type count, value_type const& value)
{
    iterator current = dest;
    try
    {
        while (count > 0)
        {
            construct(current, value);
            ++current;
            --count;
        }
    }
    catch (...)
    {
        destroy(dest, current);
        throw;
    }
    return current;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>&
Storage<Type, Allocator>::uninitialized_fill(iterator begin, iterator end, value_type const& val)
{
    iterator current = begin;
    try
    {
        while (current != end)
        {
            construct(current, val);
            ++current;
        }
    }
    catch (...)
    {
        destroy(begin, current);
        throw;
    }
    return *this;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>&
Storage<Type, Allocator>::uninitialized_default_construct(iterator begin, iterator end)
{
    iterator current = begin;
    try
    {
        while (current != end)
        {
            construct(current);
            ++current;
        }
    }
    catch (...)
    {
        destroy(begin, current);
        throw;
    }
    return *this;
}

template<class Type, class Allocator>
constexpr Storage<Type, Allocator>&
Storage<Type, Allocator>::uninitialized_move(iterator begin, iterator end, iterator dest)
{
    iterator current = dest;
    try
    {
        while (begin != end)
        {
            construct(current, pw::move(*begin));
            ++current;
            ++begin;
        }
    }
    catch (...)
    {
        destroy(dest, current);
        throw;
    }
    return *this;
}

template<class Type, class Allocator>
template<class InputIterator>
constexpr Storage<Type, Allocator>&
Storage<Type, Allocator>::uninitialized_copy(InputIterator begin, InputIterator end, iterator dest)
{
    iterator current = dest;
    try
    {
        while (begin != end)
        {
            construct(current, *begin);
            ++current;
            ++begin;
        }
    }
    catch (...)
    {
        destroy(dest, current);
        throw;
    }
    return *this;
}

} // namespace pw::internal
#endif /* INCLUDED_PW_INTERNAL_STORAGE_H */

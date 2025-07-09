#ifndef INCLUDED_PW_INTERNAL_STORAGE2_H
#define INCLUDED_PW_INTERNAL_STORAGE2_H
#endif /* INCLUDED_PW_INTERNAL_STORAGE2_H */

#include <pw/impl/allocator.h>
#include <pw/impl/allocator_traits.h>
#include <pw/impl/destroy.h>
#include <pw/impl/swap.h>
#include <pw/impl/uninitialized_move.h>

namespace pw::internal {
template<class Type, class Allocator = pw::allocator<Type>>
struct Storage2
{
    using value_type      = Type;
    using allocator_type  = Allocator;
    using size_type       = typename allocator_traits<Allocator>::size_type;
    using difference_type = typename allocator_traits<Allocator>::difference_type;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using pointer         = typename allocator_traits<Allocator>::pointer;
    using const_pointer   = typename allocator_traits<Allocator>::const_pointer;
    using iterator        = pointer;
    using const_iterator  = const_pointer;

    explicit Storage2(allocator_type const& alloc);
    ~Storage2();

    constexpr void               reserve(size_type count, std::function<void(pointer begin)> func);
    constexpr void               reserve(size_type count);
    [[nodiscard]] constexpr bool empty() const noexcept;
    constexpr pointer            begin() noexcept;
    constexpr const_pointer      begin() const noexcept;
    constexpr pointer            end() noexcept;
    constexpr const_pointer      end() const noexcept;
    constexpr Storage2&          set_size(size_type size) noexcept;
    constexpr size_type          size() const noexcept;
    constexpr size_type          allocated() const noexcept;
    constexpr allocator_type     get_allocator() const;
    constexpr void
    swap(Storage2& other) noexcept(allocator_traits<allocator_type>::propagate_on_container_swap::value ||
                                   allocator_traits<allocator_type>::is_always_equal::value);

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
Storage2<Type, Allocator>::reserve(size_type count, std::function<void(pointer begin)> func)
{
    reserve(count);
    func(begin());
    set_size(count);
}

template<class Type, class Allocator>
constexpr void
Storage2<Type, Allocator>::reserve(size_type count)
{
    pointer   tmp = allocator_traits<Allocator>::allocate(m_alloc, count);
    size_type old;

    pw::uninitialized_move(begin(), end(), tmp);
    /*
     * If a destructor throws (which is strongly discouraged) then
     * this is left in a valid but unspecified state.  The memory
     * is deallocated
     */
    try
    {
        pw::destroy(begin(), end());
    }
    catch (...)
    {
        allocator_traits<Allocator>::deallocate(m_alloc, tmp, count);
        throw;
    }
    allocator_traits<Allocator>::deallocate(m_alloc, m_begin, m_allocated);
    using pw::swap;
    swap(tmp, m_begin);
    swap(count, m_allocated);
}

template<class Type, class Allocator>
constexpr bool
Storage2<Type, Allocator>::empty() const noexcept
{
    return m_size == 0;
}

template<class Type, class Allocator>
constexpr typename Storage2<Type, Allocator>::pointer
Storage2<Type, Allocator>::begin() noexcept
{
    return m_begin;
}

template<class Type, class Allocator>
constexpr typename Storage2<Type, Allocator>::const_pointer
Storage2<Type, Allocator>::begin() const noexcept
{
    return m_begin;
}

template<class Type, class Allocator>
constexpr typename Storage2<Type, Allocator>::pointer
Storage2<Type, Allocator>::end() noexcept
{
    return m_begin + m_size;
}

template<class Type, class Allocator>
constexpr typename Storage2<Type, Allocator>::const_pointer
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
constexpr typename Storage2<Type, Allocator>::size_type
Storage2<Type, Allocator>::size() const noexcept
{
    return m_size;
}

template<class Type, class Allocator>
constexpr typename Storage2<Type, Allocator>::size_type
Storage2<Type, Allocator>::allocated() const noexcept
{
    return m_allocated;
}

template<class Type, class Allocator>
constexpr typename Storage2<Type, Allocator>::allocator_type
Storage2<Type, Allocator>::get_allocator() const
{
    return m_alloc;
}

template<class Type, class Allocator>
constexpr void
Storage2<Type, Allocator>::swap(Storage2& other) noexcept(
    pw::allocator_traits<allocator_type>::propagate_on_container_swap::value ||
    pw::allocator_traits<allocator_type>::is_always_equal::value)
{
    using pw::swap;
    if constexpr (allocator_traits<allocator_type>::propagate_on_container_swap::value)
    {
        swap(m_alloc, other.m_alloc);
    }
    swap(m_begin, other.m_begin);
    swap(m_size, other.m_size);
    swap(m_allocated, other.m_allocated);
}
} // namespace pw::internal

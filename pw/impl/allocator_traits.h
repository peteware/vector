#ifndef INCLUDED_PW_IMPL_ALLOCATOR_TRAITS_H
#define INCLUDED_PW_IMPL_ALLOCATOR_TRAITS_H

#include <pw/impl/construct_at.h>
#include <pw/impl/forward.h>
#include <pw/impl/is_empty.h>
#include <pw/impl/make_unsigned.h>
#include <pw/impl/numeric_limits.h>
#include <pw/impl/pointer_traits.h>
#include <pw/internal/detect_prop.h>

namespace pw {

template<typename T, typename = void>
constexpr bool is_defined = false;

template<typename T>
constexpr bool is_defined<T, decltype(sizeof(T), void())> = true;

template<class Alloc>
struct allocator_traits
{
    using allocator_type     = Alloc;
    using value_type         = typename Alloc::value_type;
    using pointer            = typename pointer_traits<value_type*>::pointer;
    using const_pointer      = typename pointer_traits<pointer>::template rebind<const value_type>;
    using void_pointer       = typename pointer_traits<pointer>::template rebind<void>;
    using const_void_pointer = typename pointer_traits<pointer>::template rebind<const void>;
    using difference_type    = typename pointer_traits<pointer>::difference_type;
    using size_type          = typename make_unsigned<difference_type>::type;
    using propagate_on_container_copy_assignment = decltype(internal::detect_prop_on_copy<Alloc>(0));
    using propagate_on_container_move_assignment = decltype(internal::detect_prop_on_move<Alloc>(0));
    using propagate_on_container_swap            = decltype(internal::detect_prop_on_swap<Alloc>(0));
    using is_always_equal                        = typename is_empty<Alloc>::type;

    static pointer allocate(allocator_type& alloc, size_type n) { return alloc.allocate(n); }

    static constexpr void deallocate(allocator_type& alloc, pointer p, size_type count) { alloc.deallocate(p, count); }

    template<class Type, class... Args>
    static constexpr void construct(allocator_type& alloc, Type* p, Args&&... args)
    {
        construct_at(p, pw::forward<Args>(args)...);
    }

    template<class Type>
    static void destroy(Alloc& a, Type* p)
    {
        destroy_impl(a, p);
    }
    static constexpr Alloc select_on_container_copy_construction(const Alloc& alloc)
    {
        return select_on_container_copy_construction_impl(alloc);
    }

    static constexpr size_type max_size(const Alloc& alloc) { return max_size_impl(alloc); }

private:
    template<class Type>
    static auto max_size_impl(const Type& alloc) -> decltype(alloc.max_size())
    {
        return alloc.max_size();
    }

    static constexpr size_t max_size_impl(const Alloc& alloc)
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    template<class Type>
    static auto select_on_container_copy_construction_impl(const Type& alloc)
        -> decltype(alloc.select_on_container_copy_construction())
    {
        return alloc.select_on_container_copy_construction();
    }

    static Alloc select_on_container_copy_construction_impl(const Alloc& alloc) { return alloc; }

    template<typename A, typename T>
    static auto destroy_impl(A& a, // NOLINT(runtime/references)
                             T* p) -> decltype(a.destroy(p))
    {
        a.destroy(p);
    }

    template<typename T>
    static void destroy_impl(Alloc&, T* p)
    {
        p->~T();
    }
};

} // namespace pw
#endif /*  INCLUDED_PW_IMPL_ALLOCATOR_TRAITS_H */

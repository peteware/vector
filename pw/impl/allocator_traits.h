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

/**
 * @brief Provides a uniform interface to allocator types.
 *
 * This class template provides a standardized way to access allocator
 * functionality and type information. It defines standard type aliases
 * and static member functions that work with any allocator type, providing
 * defaults when the allocator doesn't define certain members.
 *
 * @tparam Alloc The allocator to provide traits for
 */
template<class Alloc>
struct allocator_traits
{
    using allocator_type                         = Alloc;
    using value_type                             = Alloc::value_type;
    using pointer                                = pointer_traits<value_type*>::pointer;
    using const_pointer                          = pointer_traits<pointer>::template rebind<const value_type>;
    using void_pointer                           = pointer_traits<pointer>::template rebind<void>;
    using const_void_pointer                     = pointer_traits<pointer>::template rebind<const void>;
    using difference_type                        = pointer_traits<pointer>::difference_type;
    using size_type                              = make_unsigned<difference_type>::type;
    using propagate_on_container_copy_assignment = decltype(internal::detect_prop_on_copy<Alloc>(0));
    using propagate_on_container_move_assignment = decltype(internal::detect_prop_on_move<Alloc>(0));
    using propagate_on_container_swap            = decltype(internal::detect_prop_on_swap<Alloc>(0));
    using is_always_equal                        = is_empty<Alloc>::type;

    static pointer allocate(allocator_type& alloc, size_type n)
    {
        if (n == 0)
            return nullptr;
        return alloc.allocate(n);
    }

    static constexpr void deallocate(allocator_type& alloc, pointer p, size_type count)
    {
        alloc.deallocate(p, count);
    }

    // ReSharper disable once CppDoxygenUnresolvedReference
    /**
     * @brief Constructs an object in allocated uninitialized storage.
     *
     * Constructs a Type object at the location pointed to by p using
     * the provided arguments. This function provides a uniform interface for
     * object construction that works with any allocator type.
     *
     * The construction is performed using placement `new` with perfect forwarding
     * of the provided arguments to the Type constructor. The allocator parameter
     * is ignored in the default implementation but may be used by specialized
     * allocator types that override this behavior.
     *
     * @tparam Type The type of object to construct. Must be constructible from Args.
     * @tparam Args The types of the constructor arguments (deduced).
     *
     * @param alloc The allocator instance (unused in default implementation).
     * @param p Pointer to uninitialized memory where the object will be constructed.
     *          Must point to storage suitable for an object of type `Type`.
     * @param args Arguments to forward to the Type constructor.
     *
     * @throws Any exception thrown by the Type constructor.
     *
     * @pre p must point to uninitialized memory of sufficient size and alignment for Type.
     * @post An object of type `Type` is constructed at the location p.
     *
     * @complexity Constant, plus the complexity of the Type constructor.
     *
     * @note This function uses perfect forwarding to preserve value categories
     *       of the constructor arguments.
     * @note The memory pointed to by p must have been allocated but not yet
     *       contain a constructed object.
     */
    template<class Type, class... Args>
    static constexpr void construct(allocator_type&, Type* p, Args&&... args)
    {
        pw::construct_at(p, pw::forward<Args>(args)...);
    }

    template<class Type>
    static void destroy(Alloc& a, Type* p)
    {
        destroy_impl(a, p);
    }
    static constexpr Alloc select_on_container_copy_construction(const Alloc& alloc)
    {
        return select_on_container_copy_construction_impl(alloc, 0);
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

    template<class AllocType>
    static auto select_on_container_copy_construction_impl(const AllocType& alloc, int)
        -> decltype(alloc.select_on_container_copy_construction())
    {
        return alloc.select_on_container_copy_construction();
    }

    static constexpr Alloc select_on_container_copy_construction_impl(const Alloc& alloc, ...)
    {
        return alloc;
    }

    template<typename A, typename T>
    static auto destroy_impl(A& a, // NOLINT(runtime/references)
                             T* p) -> decltype(a.destroy(p))
    {
        return a.destroy(p);
    }

    template<typename T>
    static void destroy_impl(Alloc&, T* p)
    {
        p->~T();
    }
};

} // namespace pw
#endif /*  INCLUDED_PW_IMPL_ALLOCATOR_TRAITS_H */

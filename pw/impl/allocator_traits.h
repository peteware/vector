#ifndef INCLUDED_PW_IMPL_ALLOCATOR_TRAITS_H
#define INCLUDED_PW_IMPL_ALLOCATOR_TRAITS_H

#include <pw/impl/bool_type.h>
#include <pw/impl/construct_at.h>
#include <pw/impl/forward.h>
#include <pw/impl/is_empty.h>
#include <pw/impl/make_unsigned.h>
#include <pw/impl/pointer_traits.h>

//#include <utility>

namespace pw {

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
    using propagate_on_container_copy_assignment = false_type;
    using propagate_on_container_move_assignment = false_type;
    using propagate_on_container_swap            = false_type;
    using is_always_equal                        = typename is_empty<Alloc>::type;

    static pointer allocate(allocator_type& alloc, size_type n)
    {
        return alloc.allocate(n);
    }

    static void deallocate(allocator_type& alloc, pointer p, size_type count)
    {
        alloc.deallocate(p, count);
    }

    template<class Type, class... Args>
    static constexpr void construct(allocator_type& alloc, Type* p, Args&&... args)
    {
        construct_at(p, pw::forward<Args>(args)...);
    }

    template<class Type>
    static void destroy(Alloc& a, Type* p)
    {
        p->~Type();
    }
};

} // namespace pw
#endif /*  INCLUDED_PW_IMPL_ALLOCATOR_TRAITS_H */

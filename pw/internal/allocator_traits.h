#ifndef INCLUDED_PW_INTERNAL_ALLOCATOR_TRAITS_H
#define INCLUDED_PW_INTERNAL_ALLOCATOR_TRAITS_H
#include <pw/memory>
#include <pw/internal/make_unsigned.h>
#include <pw/internal/bool_type.h>
#include <pw/internal/is_empty.h>
#include <pw/internal/pointer_traits.h>

namespace pw { namespace internal {

template<class Alloc>
struct allocator_traits
{
    using allocator_type     = Alloc;
    using value_type         = typename Alloc::value_type;
    using pointer            = typename internal::pointer_traits<value_type*>::pointer;
    using const_pointer      = typename internal::pointer_traits<pointer>::template rebind<const value_type>;
    using void_pointer       = typename internal::pointer_traits<pointer>::template rebind<void>;
    using const_void_pointer = typename internal::pointer_traits<pointer>::template rebind<const void>;
    using difference_type    = typename internal::pointer_traits<pointer>::difference_type;
    using size_type          = typename internal::make_unsigned<difference_type>::type;
    using propagate_on_container_copy_assignment = internal::false_type;
    using propagate_on_container_move_assignment = internal::false_type;
    using propagate_on_container_swap            = internal::false_type;
    using is_always_equal                        = typename internal::is_empty<Alloc>::type;
};

}} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_ALLOCATOR_TRAITS_H */

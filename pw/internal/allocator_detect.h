#ifndef INCLUDED_PW_INTERNAL_ALLOCATOR_DETECT_H
#define INCLUDED_PW_INTERNAL_ALLOCATOR_DETECT_H

#include <pw/impl/type_traits/void.h>
#include <pw/internal/extract_or.h>
#include <pw/internal/meta.h>

// Type extractors for allocator optional type aliases.
// Used with def_or_type<Getter, Alloc, Default> to implement the
// "use Alloc::X if defined, otherwise fall back to Default" pattern
// required by std::allocator_traits.

namespace pw::internal {

template<typename A>
using alloc_pointer = typename A::pointer;

template<typename A>
using alloc_const_pointer = typename A::const_pointer;

template<typename A>
using alloc_void_pointer = typename A::void_pointer;

template<typename A>
using alloc_const_void_pointer = typename A::const_void_pointer;

template<typename A>
using alloc_difference_type = typename A::difference_type;

template<typename A>
using alloc_size_type = typename A::size_type;

template<typename A>
using alloc_is_always_equal = typename A::is_always_equal;

// rebind_alloc_helper: computes rebind_alloc<T> for allocator_traits.
// Uses Alloc::rebind<U>::other if present, otherwise synthesizes via
// rebind_first_arg (replaces the first template argument of Alloc with U).
// Defined at namespace scope so both Alloc and U are template parameters,
// which is required for the partial specialisation to SFINAE correctly.
template<typename Alloc, typename U, typename = void>
struct rebind_alloc_helper
{
    using type = rebind_first_arg<Alloc, U>::type;
};

template<typename Alloc, typename U>
struct rebind_alloc_helper<Alloc, U, void_t<typename Alloc::template rebind<U>::other>>
{
    using type = typename Alloc::template rebind<U>::other;
};

} // namespace pw::internal
#endif /* INCLUDED_PW_INTERNAL_ALLOCATOR_DETECT_H */
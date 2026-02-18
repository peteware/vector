#ifndef INCLUDED_PW_IMPL_POINTER_TRAITS_H
#define INCLUDED_PW_IMPL_POINTER_TRAITS_H

#include <pw/impl/cstddef/ptrdiff.h>
#include <pw/impl/memory/addressof.h>
#include <pw/internal/meta.h>

namespace pw {

/**
 * The generic template that is used for smart ptrs.
 *
 * The specialization for <Type *> is for real ptrs.
 */
template<class Ptr>
struct pointer_traits
{
    using pointer         = Ptr;
    using element_type    = internal::element_type<Ptr>::type;
    using difference_type = internal::difference_type<Ptr>::type;

    template<typename U>
    using rebind = internal::rebind_ptr<Ptr, U>::type;

    static pointer pointer_to(element_type& r) { return Ptr::pointer_to(r); }
};

/**
 * Specialization for real pointers (Type *)
 */
template<class Type>
struct pointer_traits<Type*>
{
    using pointer         = Type*;
    using element_type    = Type;
    using difference_type = ptrdiff_t;

    template<typename U>
    using rebind = U*;

    static pointer pointer_to(element_type& r) noexcept { return addressof(r); }
};

} // namespace pw
#endif /* INCLUDED_PW_IMPL_POINTER_TRAITS_H */

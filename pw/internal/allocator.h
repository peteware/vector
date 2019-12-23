#ifndef INCLUDED_PW_INTERNAL_ALLOCATOR_H
#define INCLUDED_PW_INTERNAL_ALLOCATOR_H

#include <pw/internal/ptrdiff.h>
#include <pw/internal/size.h>
#include <pw/internal/bool_type.h>

namespace pw { namespace internal {

template<class Type>
struct allocator
{
    using value_type                             = Type;
    using size_type                              = size_t;
    using difference_type                        = ptrdiff_t;
    using is_always_equal                        = true_type;
    using propagate_on_container_move_assignment = true_type;

    Type* allocate(size_type n);
};

template<class Type>
Type*
allocator<Type>::allocate(size_type n)
{
    return static_cast<Type*>(::operator new(n * sizeof(Type)));
}

template<class Type1, class Type2>
bool
operator==(allocator<Type1> const& op1, allocator<Type2> const& op2)
{
    return true;
}

}} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_ALLOCATOR_H */

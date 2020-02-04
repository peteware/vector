#ifndef INCLUDED_PW_IMPL_ALLOCATOR_H
#define INCLUDED_PW_IMPL_ALLOCATOR_H

#include <pw/impl/bool_type.h>
#include <pw/impl/ptrdiff.h>
#include <pw/impl/size.h>

namespace pw {

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

} // namespace pw
#endif /*  INCLUDED_PW_IMPL_ALLOCATOR_H */

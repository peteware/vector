#ifndef INCLUDED_PW_IMPL_ALLOCATOR_H
#define INCLUDED_PW_IMPL_ALLOCATOR_H

#include <pw/impl/bool_type.h>
#include <pw/impl/cstddef/ptrdiff.h>
#include <pw/impl/cstddef/size.h>
#include <pw/impl/type_traits/is_constant_evaluated.h>

namespace pw {

template<class Type>
struct allocator
{
    using value_type                             = Type;
    using size_type                              = size_t;
    using difference_type                        = ptrdiff_t;
    using is_always_equal                        = true_type;
    using propagate_on_container_move_assignment = true_type;

    [[nodiscard]] constexpr Type* allocate(size_type count);
    constexpr void                deallocate(Type* ptr, size_type count);
};

// Non-member operator declarations
template<class Type1, class Type2>
bool operator==(allocator<Type1> const& op1, allocator<Type2> const& op2);

template<class Type1, class Type2>
bool operator!=(allocator<Type1> const& op1, allocator<Type2> const& op2);

// Implementation section
template<class Type>
constexpr Type*
// ReSharper disable once CppMemberFunctionMayBeStatic
allocator<Type>::allocate(size_type count)
{
    return static_cast<Type*>(operator new(count * sizeof(Type)));
}

template<class Type>
constexpr void
// ReSharper disable once CppMemberFunctionMayBeStatic
allocator<Type>::deallocate(Type* ptr, size_type)
{
    if (is_constant_evaluated())
    {
        // In constant evaluation, we cannot use `::operator delete` as it may not be available.
        // This is a workaround to avoid issues in constant expressions.
        return;
    }
    operator delete(static_cast<void*>(ptr));
}

template<class Type1, class Type2>
bool
operator==(allocator<Type1> const&, allocator<Type2> const&)
{
    return true;
}

template<class Type1, class Type2>
bool
operator!=(allocator<Type1> const&, allocator<Type2> const&)
{
    return false;
}

} // namespace pw
#endif /*  INCLUDED_PW_IMPL_ALLOCATOR_H */

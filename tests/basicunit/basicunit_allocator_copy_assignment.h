#ifndef INCLUDED_PW_BASICUNIT_ALLOCATOR_COPY_ASSIGNMENT_H
#define INCLUDED_PW_BASICUNIT_ALLOCATOR_COPY_ASSIGNMENT_H

#include "allocator.h"
#include <iostream>

namespace basicunit {
template<class Type>
struct allocator_copy_assignment : allocator_base<Type>
{
    using base            = allocator_base<Type>;
    using value_type      = typename base::value_type;
    using size_type       = typename base::size_type;
    using difference_type = typename base::difference_type;
    using is_always_equal = pw::false_type;
    using propagate_on_container_copy_assignment = pw::true_type;
    using propagate_on_container_move_assignment = pw::true_type;
    using propagate_on_container_swap            = pw::false_type;

    allocator_copy_assignment() = default;
    Type* allocate(size_type n);
    void  deallocate(Type* ptr, size_type count);
};

template<class Type>
Type*
allocator_copy_assignment<Type>::allocate(size_type count)
{
    return static_cast<Type*>(::operator new(count * sizeof(Type)));
}

template<class Type>
void
allocator_copy_assignment<Type>::deallocate(Type* ptr, size_type count)
{
    return ::operator delete(static_cast<void*>(ptr));
}

template<class Type1, class Type2>
bool
operator==(allocator_copy_assignment<Type1> const& op1,
           allocator_copy_assignment<Type2> const& op2)
{
    return op1.m_instance == op2.m_instance;
}

template<class Type>
std::ostream&
operator<<(std::ostream& out, allocator_copy_assignment<Type> const& alloc)
{
    out << "allocator_copy_assignment<" << typeid(Type).name() << "> instance "
        << alloc.m_instance;
    return out;
}
} // namespace basicunit
#endif /* INCLUDED_PW_BASICUNIT_ALLOCATOR_COPY_ASSIGNMENT_H */

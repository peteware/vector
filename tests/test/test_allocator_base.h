#ifndef INCLUDED_PW_TEST_ALLOCATOR_BASE_H
#define INCLUDED_PW_TEST_ALLOCATOR_BASE_H

#include <pw/impl/bool_type.h>
#include <pw/impl/ptrdiff.h>
#include <pw/impl/size.h>

#include <iostream>

namespace pw::test {

template<class Type>
struct allocator_base
{
    using value_type                             = Type;
    using size_type                              = size_t;
    using difference_type                        = ptrdiff_t;
    using propagate_on_container_copy_assignment = false_type;
    using propagate_on_container_move_assignment = false_type;
    using propagate_on_container_swap            = false_type;
    using is_always_equal                        = false_type;

    int m_instance;

    explicit allocator_base(int instance = 1);
    Type* allocate(size_type count);
    void  deallocate(Type* ptr, size_type count);

    template<class T, class... Args>
    void construct(T* p, Args&&... args);
};

template<class Type>
allocator_base<Type>::allocator_base(int instance)
    : m_instance(instance)
{
}

template<class Type>
Type*
allocator_base<Type>::allocate(size_type count)
{
    return static_cast<Type*>(operator new(count * sizeof(Type)));
}

template<class Type>
void
allocator_base<Type>::deallocate(Type* ptr, size_type)
{
    return operator delete(static_cast<void*>(ptr));
}

template<class Type>
template<class U, class... Args>
void
allocator_base<Type>::construct(U* obj, Args&&... args)
{
    uninitialized_construct_using_allocator(obj, *this, std::forward<Args>(args)...);
}

template<class Type1, class Type2>
bool
operator==(allocator_base<Type1> const& op1, allocator_base<Type2> const& op2)
{
    return op1.m_instance == op2.m_instance;
}

template<class Type>
std::ostream&
operator<<(std::ostream& out, allocator_base<Type> const& alloc)
{
    out << "allocator_base<" << typeid(Type).name() << "> instance " << alloc.m_instance;
    return out;
}
} // namespace pw::test

#endif /* INCLUDED_PW_TEST_ALLOCATOR_BASE_H */

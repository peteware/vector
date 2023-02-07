#ifndef INCLUDED_PW_BASICUNIT_ALLOCATOR_H
#define INCLUDED_PW_BASICUNIT_ALLOCATOR_H

#include <pw/impl/bool_type.h>
#include <pw/impl/ptrdiff.h>
#include <pw/impl/size.h>

namespace basicunit {

template<class Type>
struct my_allocator
{
    using value_type                             = Type;
    using size_type                              = pw::size_t;
    using difference_type                        = pw::ptrdiff_t;
    using is_always_equal                        = pw::false_type;
    using propagate_on_container_copy_assignment = pw::false_type;
    using propagate_on_container_move_assignment = pw::true_type;
    using propagate_on_container_swap            = pw::false_type;

    my_allocator();
    Type* allocate(size_type n);
    void  deallocate(Type* ptr, size_type count);

    static int s_nextInstance;
    int        m_instance;
};

template<class Type>
int my_allocator<Type>::s_nextInstance;

template<class Type>
my_allocator<Type>::my_allocator()
    : m_instance(s_nextInstance++)
{
}

template<class Type>
Type*
my_allocator<Type>::allocate(size_type count)
{
    return static_cast<Type*>(::operator new(count * sizeof(Type)));
}

template<class Type>
void
my_allocator<Type>::deallocate(Type* ptr, size_type count)
{
    return ::operator delete(static_cast<void*>(ptr));
}

template<class Type1, class Type2>
bool
operator==(my_allocator<Type1> const& op1, my_allocator<Type2> const& op2)
{
    return op1.m_instance == op2.m_instance;
}

template<class Type>
struct allocator_copy_assignment
{
    using value_type                             = Type;
    using size_type                              = pw::size_t;
    using difference_type                        = pw::ptrdiff_t;
    using is_always_equal                        = pw::false_type;
    using propagate_on_container_copy_assignment = pw::true_type;
    using propagate_on_container_move_assignment = pw::true_type;
    using propagate_on_container_swap            = pw::false_type;

    allocator_copy_assignment();
    Type* allocate(size_type n);
    void  deallocate(Type* ptr, size_type count);

    static int s_nextInstance;
    int        m_instance;
};

template<class Type>
int allocator_copy_assignment<Type>::s_nextInstance;

template<class Type>
allocator_copy_assignment<Type>::allocator_copy_assignment()
    : m_instance(s_nextInstance++)
{
}

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
operator==(allocator_copy_assignment<Type1> const& op1, allocator_copy_assignment<Type2> const& op2)
{
    return op1.m_instance == op2.m_instance;
}

} // namespace basicunit
#endif /* INCLUDED_PW_BASICUNIT_ALLOCATOR_H */

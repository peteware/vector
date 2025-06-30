#ifndef INCLUDED_PW_BASICUNIT_ALLOCATOR_MOVE_ASSIGNMENT_H
#define INCLUDED_PW_BASICUNIT_ALLOCATOR_MOVE_ASSIGNMENT_H

#include <pw/impl/bool_type.h>
#include <pw/impl/ptrdiff.h>
#include <pw/impl/size.h>

#include "allocator.h"
#include <pw/internal/meta.h>

#include <iostream>

namespace basicunit {

template<class Type>
struct allocator_move_assignment : public allocator_base<Type>
{
    using base                        = allocator_base<Type>;
    using difference_type             = typename base::difference_type;
    using size_type                   = typename base::size_type;
    using value_type                  = typename base::value_type;
    using propagate_on_container_swap = pw::false_type;

    allocator_move_assignment();
    Type* allocate(size_type n);
    void  deallocate(Type* ptr, size_type count);

    static int s_nextInstance;
    int        m_instance;
};

template<class Type>
int allocator_move_assignment<Type>::s_nextInstance;

template<class Type>
allocator_move_assignment<Type>::allocator_move_assignment()
    : m_instance(s_nextInstance++)
{
}

template<class Type>
Type*
allocator_move_assignment<Type>::allocate(size_type count)
{
    return static_cast<Type*>(::operator new(count * sizeof(Type)));
}

template<class Type>
void
allocator_move_assignment<Type>::deallocate(Type* ptr, size_type count)
{
    return ::operator delete(static_cast<void*>(ptr));
}

template<class Type1, class Type2>
bool
operator==(allocator_move_assignment<Type1> const& op1,
           allocator_move_assignment<Type2> const& op2)
{
    return op1.m_instance == op2.m_instance;
}

} // namespace basicunit
#endif /* INCLUDED_PW_BASICUNIT_ALLOCATOR_MOVE_ASSIGNMENT_H */

#ifndef INCLUDED_PW_BASICUNIT_ALLOCATOR_BASE_H
#define INCLUDED_PW_BASICUNIT_ALLOCATOR_BASE_H

#include <pw/impl/bool_type.h>
#include <pw/impl/ptrdiff.h>
#include <pw/impl/size.h>

namespace basicunit {

template<class Type>
struct allocator_base
{
    using value_type      = Type;
    using size_type       = pw::size_t;
    using difference_type = pw::ptrdiff_t;

    static int s_nextInstance;
    int        m_instance;

    allocator_base();
};

template<class Type>
int allocator_base<Type>::s_nextInstance;

template<class Type>
allocator_base<Type>::allocator_base()
    : m_instance(s_nextInstance++)
{
}

} // namespace basicunit

#endif /* INCLUDED_PW_BASICUNIT_ALLOCATOR_BASE_H */

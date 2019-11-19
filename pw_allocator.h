#ifndef PW_ALLOCATOR_H
#define PW_ALLOCATOR_H

#include <pw_cstddef.h>
#include <pw_type_traits.h>

namespace pw {
template<class Type>
struct allocator 
{
    typedef Type          value_type;
    typedef pw::size_t    size_type;
    typedef pw::ptrdiff_t difference_type;
    typedef pw::true_type is_always_equal;
};

}


#endif /*  PW_ALLOCATOR_H */

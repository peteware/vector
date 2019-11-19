#ifndef PW_ALLOCATOR_H
#define PW_ALLOCATOR_H

#include <pw_cstddef.h>
#include <pw_type_traits.h>

namespace pw {

template<class Type>
struct allocator 
{
    using value_type      = Type;
    using size_type       = pw::size_t;
    using difference_type = pw::ptrdiff_t;
    using is_always_equal = pw::true_type;
};

}


#endif /*  PW_ALLOCATOR_H */

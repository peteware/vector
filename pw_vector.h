#ifndef PW_VECTOR_H
#define PW_VECTOR_H

#include <pw_allocator.h>
#include <pw_pmr_allocator.h>

namespace pw {

template<class Type, class Allocator = pw::allocator<Type> >
class vector
{
public:
};

namespace pmr {
template <class Type>
using vector = pw::vector<Type, pw::pmr::polymorphic_allocator<Type> >;
}

}

#endif /*  PW_VECTOR_H */

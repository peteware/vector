#ifndef INCLUDED_PW_INTERNAL_ALLOCATOR_H
#define INCLUDED_PW_INTERNAL_ALLOCATOR_H

#include <pw/internal/ptrdiff.h>
#include <pw/internal/size.h>
#include <pw/internal/bool_type.h>

namespace pw { namespace internal {

template<class Type>
struct allocator
{
    using value_type                             = Type;
    using size_type                              = size_t;
    using difference_type                        = ptrdiff_t;
    using is_always_equal                        = true_type;
    using propagate_on_container_move_assignment = true_type;
};

}} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_ALLOCATOR_H */

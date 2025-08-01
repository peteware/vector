#ifndef INCLUDED_PW_BASICUNIT_ALLOCATOR_SWAPABLE_H
#define INCLUDED_PW_BASICUNIT_ALLOCATOR_SWAPABLE_H

#include "basicunit_base_allocator.h"

namespace basicunit {

/**
 * @brief An allocator that inherits from allocator_base but allows container swap propagation
 *
 * This allocator is identical to allocator_base except that it sets
 * propagate_on_container_swap to true, which means the allocator will be swapped
 * when its container is swapped.
 */
template<class Type>
struct allocator_swapable : allocator_base<Type>
{
    using propagate_on_container_swap = pw::true_type;

    explicit allocator_swapable(int instance = 1)
        : allocator_base<Type>(instance)
    {
    }
};

} // namespace basicunit

#endif /* INCLUDED_PW_BASICUNIT_ALLOCATOR_SWAPABLE_H */

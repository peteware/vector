//
// Created by Ware, Pete on 9/12/25.
//

#include <pw/impl/pmr_memory_resource.h>

namespace pw::pmr {

void*
memory_resource::allocate(size_t bytes, size_t alignment)
{
    return do_allocate(bytes, alignment);
}
void
memory_resource::deallocate(void* p, size_t bytes, size_t alignment)
{
    return do_deallocate(p, bytes, alignment);
}
bool
memory_resource::is_equal(memory_resource const& other) const noexcept
{
    return do_is_equal(other);
}

} // namespace pw::pmr
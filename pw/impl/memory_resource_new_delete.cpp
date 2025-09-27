#include <pw/impl/memory_resource_new_delete.h>

namespace pw::internal {

memory_resource_new_delete::~memory_resource_new_delete() noexcept
{
}

void*
memory_resource_new_delete::do_allocate(size_t bytes, size_t alignment)
{
    return operator new(bytes);
}
void
memory_resource_new_delete::do_deallocate(void* p, size_t bytes, size_t alignment)
{
    operator delete(p);
}

bool
memory_resource_new_delete::do_is_equal(memory_resource const& other) const noexcept
{
    return this == &other;
}
} // namespace pw::internal
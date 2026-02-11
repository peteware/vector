#include <pw/impl/memory_resource_null.h>

#include <new>

namespace pw::internal {

memory_resource_null::~memory_resource_null() noexcept
{
}

void*
memory_resource_null::do_allocate(size_t bytes, size_t alignment)
{
    throw std::bad_alloc();
}
void
memory_resource_null::do_deallocate(void* p, size_t bytes, size_t alignment)
{
}

bool
memory_resource_null::do_is_equal(pw::pmr::memory_resource const& other) const noexcept
{
    return this == &other;
}
} // namespace pw::internal
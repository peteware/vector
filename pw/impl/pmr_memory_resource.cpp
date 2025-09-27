#include <pw/impl/exchange.h>
#include <pw/impl/memory_resource_new_delete.h>
#include <pw/impl/memory_resource_null.h>
#include <pw/impl/pmr_memory_resource.h>

#include <utility>

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

memory_resource*
new_delete_resource() noexcept
{
    static internal::memory_resource_new_delete s_resource;
    return &s_resource;
}

memory_resource*
null_memory_resource() noexcept
{
    static internal::memory_resource_null s_resource;
    return &s_resource;
}

static memory_resource* s_default_resource = new_delete_resource();

memory_resource*
set_default_resource(memory_resource* r) noexcept
{
    return exchange(s_default_resource, r);
}

memory_resource*
get_default_resource() noexcept
{
    return s_default_resource;
}
} // namespace pw::pmr
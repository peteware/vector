#ifndef INCLUDED_PW_MEMORY_RESOURCE_NEW_DELETE_H
#define INCLUDED_PW_MEMORY_RESOURCE_NEW_DELETE_H

#include <pw/impl/memory_resource/pmr_memory_resource.h>

namespace pw::internal {

/**
 * A memory resource that uses new and delete for allocations and deallocations.
 *
 * This memory resource is suitable for general-purpose use where dynamic memory
 * allocation is required. It leverages the global new and delete operators to
 * manage memory, making it a straightforward choice for scenarios where custom
 * memory management strategies are not necessary.
 */
class memory_resource_new_delete : public pmr::memory_resource
{
public:
    ~memory_resource_new_delete() noexcept override;

private:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void  do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool  do_is_equal(memory_resource const& other) const noexcept override;
};

} // namespace pw::internal

#endif //INCLUDED_PW_MEMORY_RESOURCE_NEW_DELETE_H

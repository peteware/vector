#ifndef INCLUDED_PW_INTERNAL_MEMORY_RESOURCE_NULL_H
#define INCLUDED_PW_INTERNAL_MEMORY_RESOURCE_NULL_H

#include "pmr_memory_resource.h"

namespace pw::internal {
class memory_resource_null : public pw::pmr::memory_resource
{
public:
    ~memory_resource_null() noexcept override;

private:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void  do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool  do_is_equal(memory_resource const& other) const noexcept override;
};
} // namespace pw::internal
#endif //INCLUDED_PW_INTERNAL_MEMORY_RESOURCE_NULL_H

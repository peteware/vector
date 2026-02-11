#ifndef PW_PMR_MEMORY_RESOURCE_H
#define PW_PMR_MEMORY_RESOURCE_H

#include <pw/impl/cstddef/size.h>
namespace pw::pmr {

class memory_resource
{
    using max_align_t                 = double;
    static constexpr size_t max_align = alignof(max_align_t);

public:
    memory_resource()                                  = default;
    memory_resource(memory_resource const&)            = default;
    virtual ~memory_resource()                         = default;

    memory_resource& operator=(memory_resource const&) = default;

    void*            allocate(size_t bytes, size_t alignment = max_align);
    void             deallocate(void* p, size_t bytes, size_t alignment = max_align);
    bool             is_equal(memory_resource const& other) const noexcept;

private:
    virtual void* do_allocate(size_t bytes, size_t alignment)              = 0;
    virtual void  do_deallocate(void* p, size_t bytes, size_t alignment)   = 0;
    virtual bool  do_is_equal(memory_resource const& other) const noexcept = 0;
};

memory_resource* new_delete_resource() noexcept;
memory_resource* null_memory_resource() noexcept;
memory_resource* set_default_resource(memory_resource* r) noexcept;
memory_resource* get_default_resource() noexcept;
} // namespace pw::pmr

#endif //PW_PMR_MEMORY_RESOURCE_H

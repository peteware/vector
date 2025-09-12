#ifndef INCLUDED_PW_PMR_POLYMORPHIC_ALLOCATOR_H
#define INCLUDED_PW_PMR_POLYMORPHIC_ALLOCATOR_H

#include <pw/impl/byte.h>
#include <pw/impl/pmr_memory_resource.h>

namespace pw::pmr {

template<class StorageUnit = pw::byte>
class polymorphic_allocator
{
    memory_resource* m_memory_resource; // exposition only

public:
    using value_type = StorageUnit;

    polymorphic_allocator() noexcept;
    polymorphic_allocator(memory_resource* r);
    polymorphic_allocator(polymorphic_allocator const& other) = default;
    template<class U>
    polymorphic_allocator(polymorphic_allocator<U> const& other) noexcept;

    polymorphic_allocator& operator=(polymorphic_allocator const&) = delete;
    StorageUnit*           allocate(size_t size);
    void                   deallocate(StorageUnit* p, size_t size);
    void*                  allocate_bytes(size_t nbytes, size_t alignment = alignof(max_align_t));
    void                   deallocate_bytes(void* p, size_t nbytes, size_t alignment = alignof(max_align_t));
    template<class T>
    T* allocate_object(size_t n = 1);

    template<class T>
    void deallocate_object(T* p, size_t n = 1);

    template<class T, class... CtorArgs>
    T* new_object(CtorArgs&&... ctor_args);

    template<class T>
    void delete_object(T* p);

    template<class T, class... Args>
    void construct(T* p, Args&&... args);

    template<class T>
    void                  destroy(T* p);

    polymorphic_allocator select_on_container_copy_construction() const;

    memory_resource*      resource() const;

    // friends
    friend bool operator==(polymorphic_allocator const& a, polymorphic_allocator const& b) noexcept
    {
        return *a.resource() == *b.resource();
    }
};

template<class StorageUnit>
polymorphic_allocator<StorageUnit>::polymorphic_allocator() noexcept
    : m_memory_resource()
{
}
} // namespace pw::pmr

#endif /*   INCLUDED_PW_PMR_POLYMORPHIC_ALLOCATOR_H */

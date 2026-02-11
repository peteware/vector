#ifndef INCLUDED_PW_PMR_POLYMORPHIC_ALLOCATOR_H
#define INCLUDED_PW_PMR_POLYMORPHIC_ALLOCATOR_H

#include <pw/impl/byte.h>
#include <pw/impl/max_align.h>
#include <pw/impl/memory/uninitialized_construct_using_allocator.h>
#include <pw/impl/memory_resource/pmr_memory_resource.h>
#include <pw/impl/utility/forward.h>

namespace pw::pmr {

template<class StorageUnit = pw::byte>
class polymorphic_allocator
{
public:
    using value_type = StorageUnit;

    polymorphic_allocator() noexcept;
    polymorphic_allocator(memory_resource* r) noexcept;
    polymorphic_allocator(polymorphic_allocator const& other) = default;
    template<class U>
    polymorphic_allocator(polymorphic_allocator<U> const& other) noexcept;

    polymorphic_allocator& operator=(polymorphic_allocator const&) = delete;
    StorageUnit*           allocate(size_t size);
    void                   deallocate(StorageUnit* p, size_t size);
    void*                  allocate_bytes(size_t nbytes, size_t alignment = alignof(max_align_t));
    void                   deallocate_bytes(void* p, size_t nbytes, size_t alignment = alignof(max_align_t));
    polymorphic_allocator  select_on_container_copy_construction() const;
    memory_resource*       resource() const;

    template<class T>
    T* allocate_object(size_t n = 1);

    template<class T>
    void deallocate_object(T* p, size_t n = 1);

    template<class T, class... CtorArgs>
    T* new_object(CtorArgs&&... args);

    template<class T>
    void delete_object(T* p);

    template<class T, class... Args>
    void construct(T* p, Args&&... args);

    template<class T>
    void destroy(T* p);

    // friends
    friend bool operator==(polymorphic_allocator const& a, polymorphic_allocator const& b) noexcept
    {
        return *a.resource() == *b.resource();
    }

private:
    memory_resource* m_memory_resource;
};

template<class StorageUnit>
polymorphic_allocator<StorageUnit>::polymorphic_allocator() noexcept
    : m_memory_resource(get_default_resource())
{
}

template<class StorageUnit>
polymorphic_allocator<StorageUnit>::polymorphic_allocator(memory_resource* r) noexcept
    : m_memory_resource(r)
{
}
template<class StorageUnit>
template<class U>
polymorphic_allocator<StorageUnit>::polymorphic_allocator(polymorphic_allocator<U> const& other) noexcept
    : m_memory_resource(other.resource())
{
}

template<class StorageUnit>
StorageUnit*
polymorphic_allocator<StorageUnit>::allocate(size_t size)
{
    return static_cast<StorageUnit*>(resource()->allocate(size * sizeof(StorageUnit), alignof(StorageUnit)));
}

template<class StorageUnit>
void
polymorphic_allocator<StorageUnit>::deallocate(StorageUnit* p, size_t size)
{
    resource()->deallocate(p, size * sizeof(StorageUnit), alignof(StorageUnit));
}

template<class StorageUnit>
void*
polymorphic_allocator<StorageUnit>::allocate_bytes(size_t nbytes, size_t alignment)
{
    return resource()->allocate(nbytes, alignment);
}

template<class StorageUnit>
void
polymorphic_allocator<StorageUnit>::deallocate_bytes(void* p, size_t nbytes, size_t alignment)
{
    resource()->deallocate(p, nbytes, alignment);
}

template<class StorageUnit>
template<class Type>
Type*
polymorphic_allocator<StorageUnit>::allocate_object(size_t n)
{
    return static_cast<Type*>(allocate_bytes(n * sizeof(Type), alignof(Type)));
}

template<class StorageUnit>
template<class Type>
void
polymorphic_allocator<StorageUnit>::deallocate_object(Type* p, size_t n)
{
    deallocate_bytes(p, static_cast<size_t>(n * sizeof(Type)));
}

template<class StorageUnit>
template<class Type, class... Args>
Type*
polymorphic_allocator<StorageUnit>::new_object(Args&&... args)
{
    Type* p = allocate_object<Type>(1);
    try
    {
        construct(p, pw::forward<Args>(args)...);
    }
    catch (...) // catch all
    {
        deallocate_object(p, 1);
        throw;
    }
    return p;
}

template<class StorageUnit>
template<class Type>
void
polymorphic_allocator<StorageUnit>::delete_object(Type* p)
{
    destroy(p);
    deallocate_object(p, 1);
}

template<class StorageUnit>
template<class Type, class... Args>
void
polymorphic_allocator<StorageUnit>::construct(Type* p, Args&&... args)
{
    uninitialized_construct_using_allocator(p, *this, pw::forward<Args>(args)...);
}

template<class StorageUnit>
template<class Type>
void
polymorphic_allocator<StorageUnit>::destroy(Type* p)
{
    destroy_at(p);
}

template<class StorageUnit>
polymorphic_allocator<StorageUnit>
polymorphic_allocator<StorageUnit>::select_on_container_copy_construction() const
{
    return *this;
}

template<class StorageUnit>
memory_resource*
polymorphic_allocator<StorageUnit>::resource() const
{
    return m_memory_resource;
}

} // namespace pw::pmr

#endif /*   INCLUDED_PW_PMR_POLYMORPHIC_ALLOCATOR_H */

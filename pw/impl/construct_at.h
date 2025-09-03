#ifndef INCLUDED_PW_IMPL_CONSTRUCT_AT_H
#define INCLUDED_PW_IMPL_CONSTRUCT_AT_H

#include <pw/impl/forward.h>

namespace pw {

/**
 * Construct an object at a location that is uninitialized.
 *
 * - It does not allocate memory; it only constructs the object at the given address.
 * - The memory must be properly aligned and large enough for the object.
 * - You are responsible for destroying the object later (e.g., with std::destroy_at).
 *
 * @tparam Type The type of object to construct
 * @tparam Args Arbitrary arguments to pass to constructor
 * @param p The memory location
 * @param args The arguments for constructor
 * @return A
 */
template<class Type, class... Args>
constexpr Type*
construct_at(Type* p, Args&&... args)
{
    return ::new (const_cast<void*>(static_cast<void const volatile*>(p))) Type(pw::forward<Args>(args)...);
}
} // namespace pw
#endif /* INCLUDED_PW_IMPL_CONSTRUCT_AT_H */

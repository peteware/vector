#ifndef INCLUDED_PW_IMPL_CONSTRUCT_AT_H
#define INCLUDED_PW_IMPL_CONSTRUCT_AT_H

#include <pw/impl/forward.h>

namespace pw {
template<class Type, class... Args>
constexpr Type*
construct_at(Type* p, Args&&... args)
{
    return ::new (const_cast<void*>(static_cast<const volatile void*>(p))) Type(pw::forward<Args>(args)...);
}
} // namespace pw
#endif /* INCLUDED_PW_IMPL_CONSTRUCT_AT_H */

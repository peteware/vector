#ifndef PW_UNINITIALIZED_CONSTRUCT_USING_ALLOCATOR_H
#define PW_UNINITIALIZED_CONSTRUCT_USING_ALLOCATOR_H

#include <pw/impl/construct_at.h>
#include <pw/impl/forward.h>
#include <pw/impl/uses_allocator_construction_args.h>

#include <memory>

namespace pw {

template<class Type, class Alloc, class... Args>
constexpr Type*
uninitialized_construct_using_allocator(Type* p, const Alloc& alloc, Args&&... args)
{
    return std::apply([&]<class... Xs>(Xs&&... xs) { return pw::construct_at(p, pw::forward<Xs>(xs)...); },
                      pw::uses_allocator_construction_args<Type>(alloc, std::forward<Args>(args)...));
}
} // namespace pw
#endif //PW_UNINITIALIZED_CONSTRUCT_USING_ALLOCATOR_H

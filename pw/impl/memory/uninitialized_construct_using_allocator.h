#ifndef PW_UNINITIALIZED_CONSTRUCT_USING_ALLOCATOR_H
#define PW_UNINITIALIZED_CONSTRUCT_USING_ALLOCATOR_H

#include <pw/impl/construct_at.h>
#include <pw/impl/memory/uses_allocator_construction_args.h>
#include <pw/impl/utility/forward.h>

//#include <memory>

namespace pw {

template<class Type, class Alloc, class... Args>
constexpr Type*
uninitialized_construct_using_allocator(Type* p, Alloc const& alloc, Args&&... args)
{
    auto func = [&]<class... Xs>(Xs&&... xs) { return pw::construct_at(p, pw::forward<Xs>(xs)...); };

    return std::apply(func, pw::uses_allocator_construction_args<Type>(alloc, pw::forward<Args>(args)...));
}
} // namespace pw
#endif //PW_UNINITIALIZED_CONSTRUCT_USING_ALLOCATOR_H

#ifndef PW_USES_ALLOCATOR_CONSTRUCTION_ARGS_H
#define PW_USES_ALLOCATOR_CONSTRUCTION_ARGS_H

#include <pw/impl/forward.h>
#include <pw/impl/type_traits/remove_cv.h>

#include <memory>

namespace pw {

/**
 * @brief If Type does not use an allocator, return args as a tuple
 */
template<class Type, class Alloc, class... Args>
constexpr auto
uses_allocator_construction_args(Alloc const& alloc, Args&&... args) noexcept
{
    if constexpr (!std::uses_allocator_v<pw::remove_cv_t<Type>, Alloc>)
    {
        return std::forward_as_tuple(pw::forward<Args>(args)...);
    }
    else if constexpr (std::uses_allocator_v<pw::remove_cv_t<Type>, Alloc> &&
                       std::is_constructible_v<Type, std::allocator_arg_t, Alloc const&, Args...>)
    {
        return std::tuple<std::allocator_arg_t, Alloc const&, Args&&...>(
            std::allocator_arg, alloc, pw::forward<Args>(args)...);
    }
    else if constexpr (std::uses_allocator_v<remove_cv_t<Type>, Alloc> &&
                       std::is_constructible_v<Type, Args..., Alloc const&>)
    {
        return std::forward_as_tuple(pw::forward<Args>(args)..., alloc);
    }
    else if constexpr (std::uses_allocator_v<Type, Alloc>)
    {
        static_assert(
            false, "Type uses allocator but is not constructible with allocator in first or last position");
    }
    else
    {
        static_assert(std::is_constructible_v<Type, Args...>,
                      "Type does not use allocator but is not constructible with given args");
    }
}

} // namespace pw

#endif //PW_USES_ALLOCATOR_CONSTRUCTION_ARGS_H

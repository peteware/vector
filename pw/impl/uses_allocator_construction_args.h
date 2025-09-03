#ifndef PW_USES_ALLOCATOR_CONSTRUCTION_ARGS_H
#define PW_USES_ALLOCATOR_CONSTRUCTION_ARGS_H

#include <pw/impl/forward.h>

#include <memory>

namespace pw {

/**
 * @brief If Type does not use an allocator, return args as a tuple
 */
template<class Type, class Alloc, class... Args>
constexpr auto
uses_allocator_construction_args(Alloc const& alloc, Args&&... args) noexcept
    -> std::enable_if_t<!std::uses_allocator_v<Type, Alloc> && std::is_constructible_v<Type, Args...>,
                        std::tuple<Args&&...>>
{
    return std::forward_as_tuple(pw::forward<Args>(args)...);
}

/**
 * @brief If Type uses an allocator and can be constructed with allocator in
 * first position then return (allocator_arg, Alloc, Args...)
 * as a tuple
 */
template<class Type, class Alloc, class... Args>
constexpr auto
uses_allocator_construction_args(Alloc const& alloc, Args&&... args) noexcept
    -> std::enable_if_t<std::uses_allocator_v<Type, Alloc> &&
                            std::is_constructible_v<Type, std::allocator_arg_t, Alloc const&, Args...>,
                        std::tuple<std::allocator_arg_t, Alloc const&, Args&&...>>
{
    return std::tuple<std::allocator_arg_t, Alloc const&, Args&&...>(
        std::allocator_arg, alloc, std::forward<Args>(args)...);
}

/**
 * @brief If Type uses an allocator and can be constructed with allocator in
 * last position then return (Args..., Alloc)
 * as a tuple
 */
template<class Type, class Alloc, class... Args>
constexpr auto
uses_allocator_construction_args(Alloc const& alloc, Args&&... args) noexcept
    -> std::enable_if_t<std::uses_allocator_v<Type, Alloc> &&
                            std::is_constructible_v<Type, Args..., Alloc const&>,
                        std::tuple<Args&&..., Alloc const&>>
{
    return std::tuple<Args&&..., Alloc const&>(std::forward<Args>(args)..., alloc);
}

} // namespace pw

#endif //PW_USES_ALLOCATOR_CONSTRUCTION_ARGS_H

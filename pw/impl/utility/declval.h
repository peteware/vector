#ifndef INCLUDED_PW_IMPL_DECLVAL_H
#define INCLUDED_PW_IMPL_DECLVAL_H

namespace pw {

/**
 * TODO: Improve [declval()](https://en.cppreference.com/w/cpp/utility/declval.html) to use `add_rvalue_reference`
 *
 * TODO: Implement [add_rvalue_reference](https://en.cppreference.com/w/cpp/types/add_reference.html)
 * @tparam Type
 * @return
 */
template<typename Type>
Type
// ReSharper disable once CppFunctionDoesntReturnValue
declval() noexcept
{
    static_assert(sizeof(Type) == 0, "Do not instantiate");
}

} // namespace pw

#endif /*  INCLUDED_PW_IMPL_DECLVAL_H */

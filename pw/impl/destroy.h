#ifndef INCLUDED_PW_IMPL_DESTROY_H
#define INCLUDED_PW_IMPL_DESTROY_H

#include <pw/impl/addressof.h>
#include <pw/impl/destroy_at.h>

namespace pw {

/**
 * Destroys a range of objects.
 *
 * Calls the destructor of each object in the range [first, end), using
 * pw::destroy_at. This function does not deallocate memory; it only invokes
 * the destructors.
 *
 * @tparam Iterator Iterator type pointing to the objects to destroy.
 * @param first Iterator to the beginning of the range to destroy.
 * @param end Iterator to the end of the range to destroy.
 *
 * @note Similar to std::destroy (C++17 and later).
 */
template<typename Iterator>
constexpr void
destroy(Iterator first, Iterator end)
{
    while (first != end)
    {
        pw::destroy_at(pw::addressof(*first++));
    }
}

} // namespace pw

#endif /* INCLUDED_PW_IMPL_DESTROY_H */

#ifndef INCLUDED_PW_IMPL_DESTROY_AT_H
#define INCLUDED_PW_IMPL_DESTROY_AT_H

namespace pw {

/**
 * Calls destructor of the object pointed to by ptr.
 *
 * Calls the destructor of the object of type Type pointed to by ptr.
 * This function does not deallocate memory; it only invokes the destructor.
 *
 * @tparam Type The type of the object to destroy.
 * @param ptr Pointer to the object to destroy. Must not be null.
 */
template<class Type>
void
destroy_at(Type* ptr)
{
    ptr->~Type();
}

} // namespace pw

#endif /*  INCLUDED_PW_IMPL_DESTROY_AT_H */

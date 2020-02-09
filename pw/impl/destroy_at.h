#ifndef INCLUDED_PW_IMPL_DESTROY_AT_H
#define INCLUDED_PW_IMPL_DESTROY_AT_H

namespace pw {

template<class Type>
void
destroy_at(Type* ptr)
{
    ptr->~Type();
}

} // namespace pw

#endif /*  INCLUDED_PW_IMPL_DESTROY_AT_H */

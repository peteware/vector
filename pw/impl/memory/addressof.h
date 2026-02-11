#ifndef INCLUDED_PW_IMPL_ADDRESSOF_H
#define INCLUDED_PW_IMPL_ADDRESSOF_H
namespace pw {

template<class Type>
Type*
addressof(Type& location)
{
    return reinterpret_cast<Type*>(&const_cast<char&>(reinterpret_cast<char const volatile&>(location)));
}
} // namespace pw
#endif /*  INCLUDED_PW_IMPL_ADDRESSOF_H */

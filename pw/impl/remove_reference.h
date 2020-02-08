#ifndef INCLUDED_PW_IMPL_REMOVE_REFERENCE_H
#define INCLUDED_PW_IMPL_REMOVE_REFERENCE_H

namespace pw {

/// remove_reference
template<class Type>
struct remove_reference
{
    typedef Type type;
};

template<class Type>
struct remove_reference<Type&>
{
    typedef Type type;
};

template<class Type>
struct remove_reference<Type&&>
{
    typedef Type type;
};
} // namespace pw

#endif /*  INCLUDED_PW_IMPL_REMOVE_REFERENCE_H */

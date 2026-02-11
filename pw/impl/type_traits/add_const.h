#ifndef INCLUDED_ADD_CONST_H
#define INCLUDED_ADD_CONST_H

namespace pw {

template<class Type>
struct add_const
{
    typedef Type const type;
};

template<class Type>
using add_const_t = add_const<Type>::type;

} // namespace pw
#endif /* INCLUDED_ADD_CONST_H */

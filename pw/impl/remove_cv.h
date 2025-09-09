#ifndef INCLUDED_PW_IMPL_REMOVE_CV_H
#define INCLUDED_PW_IMPL_REMOVE_CV_H

namespace pw {
template<class Type>
struct remove_cv
{
    typedef Type type;
};
template<class Type>
struct remove_cv<Type const>
{
    typedef Type type;
};
template<class Type>
struct remove_cv<Type volatile>
{
    typedef Type type;
};
template<class Type>
struct remove_cv<Type const volatile>
{
    typedef Type type;
};
template<class Type>
using remove_cv_t = remove_cv<Type>::type;

template<class Type>
struct remove_const
{
    typedef Type type;
};
template<class Type>
struct remove_const<Type const>
{
    typedef Type type;
};
template<class Type>
using remove_const_t = typename remove_const<Type>::type;

template<class Type>
struct remove_volatile
{
    typedef Type type;
};
template<class Type>
struct remove_volatile<Type volatile>
{
    typedef Type type;
};
template<class Type>
using remove_volatile_t = typename remove_volatile<Type>::type;
} // namespace pw
#endif //INCLUDED_PW_IMPL_REMOVE_CV_H

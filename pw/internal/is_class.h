#ifndef INCLUDED_PW_INTERNAL_IS_CLASS_H
#define INCLUDED_PW_INTERNAL_IS_CLASS_H

#include <pw/internal/integral_constant.h>

namespace pw { namespace internal {

namespace is_class_impl {
template<class Type>
char test(int Type::*);

template<class Type>
int test(...);
} // namespace is_class_impl

template<class Type>
struct is_class : public integral_constant<bool, sizeof(is_class_impl::test<Type>(0)) == 1>
{
};

}} // namespace pw::internal

#endif /* INCLUDED_PW_INTERNAL_IS_CLASS_H */

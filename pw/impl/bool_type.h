#ifndef INCLUDED_PW_IMPL_BOOL_TYPE_H
#define INCLUDED_PW_IMPL_BOOL_TYPE_H

#include <pw/impl/type_traits/integral_constant.h>

namespace pw {

using true_type  = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

} // namespace pw
#endif /* INCLUDED_PW_IMPL_BOOL_TYPE_H */

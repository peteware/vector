#ifndef INCLUDED_PW_INTERNAL_BOOL_TYPE_H
#define INCLUDED_PW_INTERNAL_BOOL_TYPE_H

#include <pw/internal/integral_constant.h>

namespace pw { namespace internal {

using true_type  = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

}} // namespace pw
#endif /* INCLUDED_PW_INTERNAL_BOOL_TYPE_H */

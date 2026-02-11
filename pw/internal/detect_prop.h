#ifndef INCLUDED_PW_INTERNAL_DETECT_PROP_H_
#define INCLUDED_PW_INTERNAL_DETECT_PROP_H_

#include <pw/impl/type_traits/bool_type.h>

namespace pw::internal {
template<class Alloc>
auto detect_prop_on_copy(int) -> Alloc::propagate_on_container_copy_assignment;
template<class Alloc>
auto detect_prop_on_copy(long) -> false_type;

template<class Alloc>
auto detect_prop_on_move(int) -> Alloc::propagate_on_container_move_assignment;
template<class Alloc>
auto detect_prop_on_move(long) -> false_type;

template<class Alloc>
auto detect_prop_on_swap(int) -> Alloc::propagate_on_container_swap;
template<class Alloc>
auto detect_prop_on_swap(long) -> false_type;
} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_DETECT_PROP_H_ */

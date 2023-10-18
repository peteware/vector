#ifndef _INCLUDED_PW_INTERNAL_DETECT_PROP_H_
#define _INCLUDED_PW_INTERNAL_DETECT_PROP_H_

namespace pw { namespace internal {
template<class Alloc>
auto detect_prop_on_copy(int) -> typename Alloc::propagate_on_container_copy_assignment;
template<class Alloc>
auto detect_prop_on_copy(long) -> false_type;

template<class Alloc>
auto detect_prop_on_move(int) -> typename Alloc::propagate_on_container_move_assignment;
template<class Alloc>
auto detect_prop_on_move(long) -> false_type;

template<class Alloc>
auto detect_prop_on_swap(int) -> typename Alloc::propagate_on_container_swap;
template<class Alloc>
auto detect_prop_on_swap(long) -> false_type;
}} // namespace pw::internal
#endif /* _INCLUDED_PW_INTERNAL_DETECT_PROP_H_ */

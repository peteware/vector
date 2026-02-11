#ifndef INCLUDED_PW_IMPL_VOID_H
#define INCLUDED_PW_IMPL_VOID_H

namespace pw {
/**
 * Success if all the types are valid.
 */
template<class...>
using void_t = void;

} // namespace pw
#endif /*  INCLUDED_PW_IMPL_VOID_H */

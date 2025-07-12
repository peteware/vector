//
// Created by Ware, Pete on 7/11/25.
//

#ifndef INCLUDED_PW_IMPL_IS_BASE_OF_H
#define INCLUDED_PW_IMPL_IS_BASE_OF_H
#include <type_traits>

namespace pw {
template<class Base, class Derived>
using is_base_of = std::is_base_of<Base, Derived>;
}

#endif //INCLUDED_PW_IMPL_IS_BASE_OF_H

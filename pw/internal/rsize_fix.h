//
// This fixes a problem on macOS when compiling
// with g++15:
//
// /Library/Developer/CommandLineTools/SDKs/MacOSX15.sdk/usr/include/_string.h:176:48: error: 'rsize_t' has not been declared; did you mean 'ssize_t'?
//

#ifndef PW_INTERNAL_RSIZE_FIX_H
#define PW_INTERNAL_RSIZE_FIX_H

#include <pw/impl/size.h>

#if __has_include(<sys/_types/_rsize_t.h>)
#include <sys/_types/_rsize_t.h>
#else
// Define rsize_t if not available
#if !defined(RSIZE_T_DEFINED) // NOLINT(*-reserved-identifier)
#define _RSIZE_T_DEFINED
using rsize_t = pw::size_t;
#endif
#endif

#endif //PW_INTERNAL_RSIZE_FIX_H

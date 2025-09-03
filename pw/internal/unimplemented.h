#ifndef INCLUDED_PW_INTERNAL_UNIMPLEMENTED_H
#define INCLUDED_PW_INTERNAL_UNIMPLEMENTED_H
#include <stdexcept>

namespace pw::internal {

/**
 * Exception to indicate that a function is unimplemented.
 */
class Unimplemented final : public std::runtime_error
{
public:
    explicit Unimplemented(char const* msg)
        : std::runtime_error(msg)
    {
    }
};
} // namespace pw::internal
#endif //INCLUDED_PW_INTERNAL_UNIMPLEMENTED_H

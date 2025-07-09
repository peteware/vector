#ifndef PW_INTERNAL_UNIMPLEMENTED_H
#define PW_INTERNAL_UNIMPLEMENTED_H
#include <stdexcept>

namespace pw::internal {

/**
 * Exception to indicate that a function is unimplemented.
 */
class Unimplemented final : public std::runtime_error
{
public:
    explicit Unimplemented(const char* msg)
        : std::runtime_error(msg)
    {
    }
};
} // namespace pw::internal
#endif //PW_INTERNAL_UNIMPLEMENTED_H

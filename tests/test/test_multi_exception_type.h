#ifndef PW_TEST_MULTI_EXCEPTION_TYPE_H
#define PW_TEST_MULTI_EXCEPTION_TYPE_H

#include <new>
#include <stdexcept>

namespace pw::test {

class MultiExceptionType
{
public:
    static int  construction_count;
    static bool throw_bad_alloc;
    static bool throw_runtime_error;

    MultiExceptionType()
    {
        ++construction_count;
        if (throw_bad_alloc)
        {
            --construction_count;
            throw std::bad_alloc();
        }
        if (throw_runtime_error)
        {
            --construction_count;
            throw std::runtime_error("test");
        }
    }

    ~MultiExceptionType() { --construction_count; }

    static void reset()
    {
        construction_count  = 0;
        throw_bad_alloc     = false;
        throw_runtime_error = false;
    }
};

// Static member definitions
inline int  MultiExceptionType::construction_count  = 0;
inline bool MultiExceptionType::throw_bad_alloc     = false;
inline bool MultiExceptionType::throw_runtime_error = false;

} // namespace pw::test

#endif // PW_TEST_MULTI_EXCEPTION_TYPE_H
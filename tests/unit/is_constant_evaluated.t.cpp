#include <pw/impl/type_traits/is_constant_evaluated.h>

#include <catch2/catch_test_macros.hpp>

// Template for testing non-type parameters
template<int N>
struct TestTemplate
{
    static constexpr int value = N;
};

// Helper function to test runtime evaluation
// ReSharper disable once CppDFAConstantFunctionResult
int
runtime_function()
{
    // ReSharper disable once CppDFAUnreachableCode
    return pw::is_constant_evaluated() ? 100 : 200;
}

// Helper constexpr function to test compile-time evaluation
// ReSharper disable once CppDFAConstantFunctionResult
constexpr int
constexpr_function()
{
    return pw::is_constant_evaluated() ? 100 : 200;
}

// Helper consteval function (C++20) to test guaranteed compile-time evaluation
// ReSharper disable once CppDFAConstantFunctionResult
consteval int
consteval_function()
{
    // ReSharper disable once CppDFAUnreachableCode
    return pw::is_constant_evaluated() ? 100 : 200;
}

// Test variable initialization contexts
constexpr int compile_time_var = pw::is_constant_evaluated() ? 300 : 400;
int           runtime_var      = pw::is_constant_evaluated() ? 300 : 400;

SCENARIO("is_constant_evaluated behavior", "[is_constant_evaluated]")
{
    GIVEN("Runtime evaluation contexts")
    {
        WHEN("called directly at runtime")
        {
            bool result = pw::is_constant_evaluated();
            THEN("it returns false")
            {
                REQUIRE_FALSE(result);
            }
        }

        WHEN("called in a runtime function")
        {
            int result = runtime_function();
            THEN("it returns the runtime value")
            {
                REQUIRE(result == 200); // Not constant evaluated
            }
        }

        WHEN("called through a constexpr function at runtime")
        {
            int result = constexpr_function();
            THEN("it returns the runtime value")
            {
                REQUIRE(result == 200); // Not constant evaluated at runtime
            }
        }
    }

    GIVEN("Compile-time evaluation contexts")
    {
        WHEN("used in constexpr variable initialization")
        {
            THEN("it returns true for compile-time initialization")
            {
                REQUIRE(compile_time_var == 300); // Constant evaluated (true branch)
            }
        }

        WHEN("used in runtime variable initialization")
        {
            THEN("it also returns true for global variable initialization")
            {
                // Global variable initialization is also constant-evaluated
                REQUIRE(runtime_var == 300); // Also constant evaluated
            }
        }

        WHEN("called in consteval function")
        {
            constexpr int result = consteval_function();
            THEN("it returns true")
            {
                REQUIRE(result == 100); // Always constant evaluated in consteval
            }
        }

        WHEN("used in static_assert context")
        {
            // This tests that is_constant_evaluated() works in constant expressions
            static_assert(pw::is_constant_evaluated(), "Should be constant evaluated in static_assert");
            THEN("compilation succeeds")
            {
                REQUIRE(true); // If we get here, static_assert passed
            }
        }
    }

    GIVEN("Template and constexpr contexts")
    {
        WHEN("used in constexpr if")
        {
            // ReSharper disable once CppDFAConstantFunctionResult
            constexpr auto test_constexpr_if = []() constexpr {
                if constexpr (pw::is_constant_evaluated())
                {
                    return 500;
                }
                else
                {
                    // ReSharper disable once CppDFAUnreachableCode
                    return 600;
                }
            };

            constexpr int compile_result = test_constexpr_if();
            int           runtime_result = test_constexpr_if();

            THEN("both contexts should work correctly")
            {
                // Note: constexpr if is evaluated at compile time regardless
                REQUIRE(compile_result == 500);
                REQUIRE(runtime_result == 500);
            }
        }

        WHEN("used in immediate function context")
        {
            auto           test_immediate      = []() constexpr { return pw::is_constant_evaluated(); };

            constexpr bool compile_time_result = test_immediate();
            bool           runtime_result      = test_immediate();

            THEN("different contexts give different results")
            {
                REQUIRE(compile_time_result == true); // Constant evaluated
                REQUIRE(runtime_result == false);     // Not constant evaluated
            }
        }
    }

    GIVEN("Array size and template parameter contexts")
    {
        WHEN("used as array size")
        {
            // ReSharper disable once CppDFAUnreachableCode
            constexpr int array_size = pw::is_constant_evaluated() ? 5 : 10;
            // ReSharper disable once CppTooWideScope
            int test_array[array_size];

            THEN("array is sized based on constant evaluation")
            {
                REQUIRE(sizeof(test_array) == 5 * sizeof(int)); // Should be 5, not 10
            }
        }

        WHEN("used in template non-type parameter")
        {
            // ReSharper disable once CppDFAUnreachableCode
            constexpr int param_value = pw::is_constant_evaluated() ? 42 : 84;
            using TestType            = TestTemplate<param_value>;

            THEN("template parameter uses constant evaluation result")
            {
                REQUIRE(TestType::value == 42); // Should be 42, not 84
            }
        }
    }

    GIVEN("Nested evaluation contexts")
    {
        WHEN("function is called both at compile time and runtime")
        {
            auto nested_test = [](bool expected_compile_time) constexpr {
                bool is_constexpr = pw::is_constant_evaluated();
                return is_constexpr == expected_compile_time;
            };

            constexpr bool compile_time_test = nested_test(true);
            bool           runtime_test      = nested_test(false);

            THEN("each context evaluates correctly")
            {
                REQUIRE(compile_time_test == true);
                REQUIRE(runtime_test == true);
            }
        }
    }
}

SCENARIO("is_constant_evaluated edge cases", "[is_constant_evaluated][edge_cases]")
{
    GIVEN("Complex evaluation scenarios")
    {
        WHEN("used in ternary operator contexts")
        {
            // ReSharper disable once CppDFAConstantFunctionResult
            constexpr auto ternary_test = []() constexpr {
                return pw::is_constant_evaluated() ? (pw::is_constant_evaluated() ? 111 : 222)
                                                   : pw::is_constant_evaluated()  ? 333
                                                   : 444;
            };

            constexpr int compile_result = ternary_test();
            int           runtime_result = ternary_test();

            THEN("nested ternary works correctly")
            {
                REQUIRE(compile_result == 111); // true ? (true ? 111 : 222) : ...
                REQUIRE(runtime_result == 444); // false ? ... : (false ? 333 : 444)
            }
        }

        WHEN("function returns is_constant_evaluated result directly")
        {
            constexpr auto direct_return  = []() constexpr { return pw::is_constant_evaluated(); };

            constexpr bool compile_result = direct_return();
            bool           runtime_result = direct_return();

            THEN("direct return works as expected")
            {
                REQUIRE(compile_result == true);
                REQUIRE(runtime_result == false);
            }
        }
    }

    GIVEN("Exception and error handling contexts")
    {
        WHEN("used in noexcept specification context")
        {
            // This tests that is_constant_evaluated is noexcept
            static_assert(noexcept(pw::is_constant_evaluated()), "is_constant_evaluated should be noexcept");

            THEN("function is marked noexcept")
            {
                REQUIRE(true); // If we get here, static_assert passed
            }
        }
    }
}
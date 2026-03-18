#pragma once

// TestFramework.h — минимальный фреймворк для тестирования.
// Макросы TEST, ASSERT_EQ, ASSERT_TRUE, ASSERT_STREQ, ASSERT_THROW.

#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <functional>

namespace test {

struct TestCase {
    std::string group;
    std::string name;
    std::function<void()> func;
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> cases;
    return cases;
}

struct TestRegistrar {
    TestRegistrar(const char* group, const char* name, std::function<void()> func) {
        registry().push_back({group, name, std::move(func)});
    }
};

inline int runAll() {
    int passed = 0, failed = 0;
    std::string currentGroup;

    for (auto& tc : registry()) {
        if (tc.group != currentGroup) {
            currentGroup = tc.group;
            std::cout << "\n=== " << currentGroup << " ===\n";
        }

        try {
            tc.func();
            ++passed;
            std::cout << "  [OK] " << tc.name << "\n";
        } catch (const std::exception& ex) {
            ++failed;
            std::cerr << "  [FAIL] " << tc.name << ": " << ex.what() << "\n";
        }
    }

    std::cout << "\n--- Итог: " << passed << " passed, " << failed << " failed ---\n";
    return failed > 0 ? 1 : 0;
}

class AssertionError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

} // namespace test

#define TEST(group, name) \
    static void test_##group##_##name(); \
    static test::TestRegistrar reg_##group##_##name(#group, #name, test_##group##_##name); \
    static void test_##group##_##name()

#define ASSERT_EQ(expected, actual) do { \
    auto _e = (expected); auto _a = (actual); \
    if (_e != _a) { \
        std::ostringstream _ss; \
        _ss << "ASSERT_EQ failed at " << __FILE__ << ":" << __LINE__ \
            << " — expected: " << _e << ", actual: " << _a; \
        throw test::AssertionError(_ss.str()); \
    } \
} while(0)

#define ASSERT_TRUE(expr) do { \
    if (!(expr)) { \
        std::ostringstream _ss; \
        _ss << "ASSERT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
            << " — expression: " #expr; \
        throw test::AssertionError(_ss.str()); \
    } \
} while(0)

#define ASSERT_FALSE(expr) do { \
    if (expr) { \
        std::ostringstream _ss; \
        _ss << "ASSERT_FALSE failed at " << __FILE__ << ":" << __LINE__ \
            << " — expression: " #expr; \
        throw test::AssertionError(_ss.str()); \
    } \
} while(0)

#define ASSERT_STREQ(expected, actual) do { \
    auto _e = (expected); auto _a = (actual); \
    if (std::strcmp(_e, _a) != 0) { \
        std::ostringstream _ss; \
        _ss << "ASSERT_STREQ failed at " << __FILE__ << ":" << __LINE__ \
            << " — expected: \"" << _e << "\", actual: \"" << _a << "\""; \
        throw test::AssertionError(_ss.str()); \
    } \
} while(0)

#define ASSERT_FLOAT_EQ(expected, actual) do { \
    auto _e = (expected); auto _a = (actual); \
    if (std::abs(_e - _a) > 1e-6f) { \
        std::ostringstream _ss; \
        _ss << "ASSERT_FLOAT_EQ failed at " << __FILE__ << ":" << __LINE__ \
            << " — expected: " << _e << ", actual: " << _a; \
        throw test::AssertionError(_ss.str()); \
    } \
} while(0)

#define ASSERT_THROW(expr, exception_type) do { \
    bool _caught = false; \
    try { expr; } \
    catch (const exception_type&) { _caught = true; } \
    catch (...) {} \
    if (!_caught) { \
        std::ostringstream _ss; \
        _ss << "ASSERT_THROW failed at " << __FILE__ << ":" << __LINE__ \
            << " — expected " #exception_type " was not thrown"; \
        throw test::AssertionError(_ss.str()); \
    } \
} while(0)

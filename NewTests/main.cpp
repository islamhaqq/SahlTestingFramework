#include <cassert>
#include <string>

struct TestState {
    int total = 0;
    int passed = 0;
};

void testBoolean(bool assertion, TestState &state);

std::string getTotalTestCountString(const TestState state);

/**
 * Requirements for custom testing framework:
 * 2. Output runtime for each test
 * 3. Output runtime for all tests
 * 4. Output number of tests
 * 5. Output number of failed tests
 * 6. Output number of passed tests
 * 7. Output error message for failed tests
 */
int main()
{
    int expectedTestCount = 4;
    int expectedPassedTests = 3;
    int expectedFailedTests = 1;
    TestState testState{};

    // Production code
    testBoolean(true == true, testState);
    testBoolean(1 == 1, testState);
    testBoolean(1 != 2, testState);
    testBoolean(1 == 3, testState);

    // Has correct test count (pass, fail, total)
    assert(testState.total == expectedTestCount);
    assert(testState.passed == expectedPassedTests);
    assert(testState.total - testState.passed == expectedFailedTests);

    // Outputs total tests
    const auto &expected = std::string("Total Tests: 4");
    const auto &actual1 = getTotalTestCountString(testState);
    assert(actual1 == expected);

    testBoolean(15 + 100 - 10 == 100 + 5, testState);
    assert(testState.total == 5);
    assert(testState.passed == 4);
    assert(testState.total - testState.passed == 1);
    const auto &expected2 = "Total Tests: 5";
    const auto &actual2 = getTotalTestCountString(testState);
    assert(actual2 == expected2);

    testBoolean(95 + 5 == 100, testState);
    auto expected3 = "Total Tests: 6";
    std::string actual3 = getTotalTestCountString(testState);
    assert(actual3 == expected3);

    return 0;
}

std::string getTotalTestCountString(const TestState state) {
    return std::string("Total Tests: ") + std::to_string(state.total);
}

void testBoolean(bool assertion, TestState &state) {
    state.total += 1;
    state.passed += assertion;
}

#include <cassert>
#include <string>

struct TestState {
    int total = 0;
    int passed = 0;

    static bool checkTestStateIsEqual(const int expectedTestCount, const int expectedPassedTests, const TestState testState) {
        auto totalIsEqual = testState.total == expectedTestCount;
        auto passedIsEqual = testState.passed == expectedPassedTests;
        return totalIsEqual && passedIsEqual;
    }
};

void testBoolean(bool assertion, TestState &state);

std::string getTotalTestCountString(const TestState state);

const bool checkTestStateIsEqual(const int expectedTestCount, const int expectedPassedTests, const TestState testState);

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
    int expectedFailedTests = 1;
    TestState testState{};

    // Production code
    testBoolean(true == true, testState);
    testBoolean(1 == 1, testState);
    testBoolean(1 != 2, testState);
    testBoolean(1 == 3, testState);

    // Has correct test count (pass, fail, totalIs5)
    assert(TestState::checkTestStateIsEqual(4, 3, testState));
    assert(testState.total - testState.passed == expectedFailedTests);

    // Outputs totalIs5 tests
    const auto &expected = std::string("Total Tests: 4");
    const auto &actual1 = getTotalTestCountString(testState);
    assert(actual1 == expected);

    testBoolean(15 + 100 - 10 == 100 + 5, testState);
    assert(TestState::checkTestStateIsEqual(5, 4, testState));
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

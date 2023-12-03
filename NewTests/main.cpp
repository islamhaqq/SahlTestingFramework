#include <cassert>
#include <cstring>
#include <string>

struct TestState {
    int totalTestCount = 0;
    int totalPassedTests = 0;
};

void runTest(bool assertion, TestState &state);

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
    TestState state{};

    // Production code
    runTest(true == true, state);
    runTest(1 == 1, state);
    runTest(1 != 2, state);
    runTest(1 == 3, state);

    // Has correct test count (pass, fail, total)
    assert(state.totalTestCount == expectedTestCount);
    assert(state.totalPassedTests == expectedPassedTests);
    assert(state.totalTestCount - state.totalPassedTests == expectedFailedTests);

    // Outputs total tests
    const auto &expected = std::string("Total Tests: 4");
    const auto &actual1 = getTotalTestCountString(state);
    assert(actual1 == expected);

    runTest(15 + 100 - 10 == 100 + 5, state);
    assert(state.totalTestCount == 5);
    assert(state.totalPassedTests == 4);
    assert(state.totalTestCount - state.totalPassedTests == 1);
    const auto &expected2 = "Total Tests: 5";
    const auto &actual2 = getTotalTestCountString(state);
    assert(actual2 == expected2);

    runTest(95 + 5 == 100, state);
    auto expected3 = "Total Tests: 6";
    std::string actual3 = getTotalTestCountString(state);
    assert(actual3 == expected3);

    return 0;
}

std::string getTotalTestCountString(const TestState state) {
    std::string prefix = std::string("Total Tests: ");
    auto totalTests3 = std::to_string(state.totalTestCount);
    auto actual3 = prefix + totalTests3;
    return actual3;
}

void runTest(bool assertion, TestState &state) {
    state.totalTestCount += 1;
    state.totalPassedTests += assertion;
}

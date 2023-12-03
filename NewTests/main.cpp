#include <cassert>

struct TestState {
    int totalTestCount = 0;
    int totalPassedTests = 0;
};

void getState(TestState &state, bool assertion);

/**
 * Requirements for custom testing framework:
 * 1. Assert
 * 2. Output runtime for each test
 * 3. Output runtime for all tests
 * 4. Output number of tests
 * 5. Output number of failed tests
 * 6. Output number of passed tests
 * 7. Output error message for failed tests
 */
int main()
{
     TestState state{};

    // Production code

    getState(state, true == true);
    getState(state, 1 == 1);
    getState(state, 1 != 2);
    getState(state, 1 == 3);

    // Assertions
    int expectedTestCount = 4;
    assert(state.totalTestCount == expectedTestCount);

    int expectedPassedTests = 3;
    assert(state.totalPassedTests == expectedPassedTests);

    int expectedFailedTests = 1;
    assert(state.totalTestCount - state.totalPassedTests == expectedFailedTests);

    return 0;
}

void getState(TestState &state, bool assertion) {
    state.totalTestCount += 1;
    state.totalPassedTests += assertion;
}

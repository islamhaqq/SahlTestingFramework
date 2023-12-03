#include <cassert>

void foo(bool assertion, int &totalTestCount, int &totalPassedTests);

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
    struct {
        int totalTestCount = 0;
        int totalPassedTests = 0;
    } state;

    // Production code

    foo(true == true, state.totalTestCount, state.totalPassedTests);
    foo(1 == 1, state.totalTestCount, state.totalPassedTests);
    foo(1 != 2, state.totalTestCount, state.totalPassedTests);
    foo(1 == 3, state.totalTestCount, state.totalPassedTests);

    // Assertions
    int expectedTestCount = 4;
    assert(state.totalTestCount == expectedTestCount);

    int expectedPassedTests = 3;
    assert(state.totalPassedTests == expectedPassedTests);

    int expectedFailedTests = 1;
    assert(state.totalTestCount - state.totalPassedTests == expectedFailedTests);

    return 0;
}

void foo(bool assertion, int &totalTestCount, int &totalPassedTests) {
    totalTestCount += 1;
    totalPassedTests += assertion;
}

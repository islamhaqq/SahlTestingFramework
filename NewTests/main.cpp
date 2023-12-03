#include <cassert>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

struct TestState {
    int total = 0;
    int passed = 0;
};

void testBoolean(bool assertion, TestState &state);

std::string getTotalTestCountString(const TestState state);

static bool checkTestStateIsEqual(const int expectedTestCount, const int expectedPassedTests, const TestState testState);

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
    assert(checkTestStateIsEqual(4, 3, testState));
    assert(testState.total - testState.passed == expectedFailedTests);

    // Outputs totalIs5 tests
    const auto &expected = std::string("Total Tests: 4");
    const auto &actual1 = getTotalTestCountString(testState);
    assert(actual1 == expected);

    testBoolean(15 + 100 - 10 == 100 + 5, testState);
    assert(checkTestStateIsEqual(5, 4, testState));
    assert(testState.total - testState.passed == 1);
    const auto &expected2 = "Total Tests: 5";
    const auto &actual2 = getTotalTestCountString(testState);
    assert(actual2 == expected2);

    testBoolean(95 + 5 == 100, testState);
    assert(checkTestStateIsEqual(6, 5, testState));
    assert(testState.total - testState.passed == 1);
    auto expected3 = "Total Tests: 6";
    std::string actual3 = getTotalTestCountString(testState);
    assert(actual3 == expected3);


    // ==================== Parallelization ====================


    int expectedDuration = 4;
    int individualRuntime = 1;
    int threads = 4;

    auto starTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < threads; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(individualRuntime));
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    int finalDuration = std::chrono::duration_cast<std::chrono::seconds>(endTime - starTime).count();
    std::cout << "Runtime: " << finalDuration << std::endl;

    assert(finalDuration == expectedDuration);

    int expectedDuration2 = 1;
    int individualRuntime2 = 1;

    auto starTime2 = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threadVector;
    for (int i = 0; i < threads; i++) {
        threadVector.emplace_back([individualRuntime2]() {
            std::this_thread::sleep_for(std::chrono::seconds(individualRuntime2));
        });
    }
    for (auto &thread : threadVector) {
        thread.join();
    }
    auto endTime2 = std::chrono::high_resolution_clock::now();
    int finalDuration2 = std::chrono::duration_cast<std::chrono::seconds>(endTime2 - starTime2).count();
    std::cout << "Runtime: " << finalDuration2 << std::endl;

    assert(finalDuration2 == expectedDuration2);

    // ==================== End ====================

    return 0;
}

std::string getTotalTestCountString(const TestState state) {
    return std::string("Total Tests: ") + std::to_string(state.total);
}

void testBoolean(bool assertion, TestState &state) {
    state.total += 1;
    state.passed += assertion;
}

bool checkTestStateIsEqual(const int expectedTestCount, const int expectedPassedTests, const TestState testState) {
    return testState.total == expectedTestCount && testState.passed == expectedPassedTests;
}
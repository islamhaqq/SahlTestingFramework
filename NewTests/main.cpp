#include <cassert>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <queue>
#include <mutex>

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

    // Single threaded
    int expectedDuration = 1000;
    int individualRuntime = 250;
    int threads = 4;
    int tolerance = 50;

    auto starTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < threads; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds (individualRuntime));
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    int finalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - starTime).count();
    std::cout << "Runtime: " << finalDuration << "ms" << std::endl;

    assert(finalDuration - expectedDuration >= -tolerance && finalDuration - expectedDuration <= tolerance);

    // Static parallelization -- 4 threads
    int expectedDuration2 = 250;
    int individualRuntime2 = 250;

    auto starTime2 = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threadVector;
    threadVector.reserve(threads);
    for (int i = 0; i < threads; i++) {
        threadVector.emplace_back([individualRuntime2]() {
            std::this_thread::sleep_for(std::chrono::milliseconds (individualRuntime2));
        });
    }
    for (auto &thread : threadVector) {
        thread.join();
    }
    auto endTime2 = std::chrono::high_resolution_clock::now();
    int finalDuration2 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime2 - starTime2).count();
    std::cout << "Runtime: " << finalDuration2 << "ms" << std::endl;

    assert(finalDuration2 - expectedDuration2 >= -tolerance && finalDuration2 - expectedDuration2 <= tolerance);

    // Dynamic parallelization -- 16 threads -- 64 tasks -- tolerance 100ms (unoptimized -- more tests with reduced tolerance)
    int expectedDuration3 = 100;
    int tolerance3 = 100;
    int threads16 = 16;

    std::vector<int> tasks = {100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5,
                              100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5,
                              100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5,
                              100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5};
    std::atomic<int> answer;
    answer = 0;
    const auto startime3 = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threadVector3;
    threadVector3.reserve(threads);
    std::mutex queueMutex;
    std::queue<int> taskQueue;
    for (int task : tasks) {
        taskQueue.push(task);
    }
    for (int i = 0; i < threads16; i++) {
        threadVector3.emplace_back([tasks, &answer, &taskQueue, &queueMutex]() {
            while (true) {
                int task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    if (taskQueue.empty()) break;
                    task = taskQueue.front();
                    taskQueue.pop();
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(task));
                answer += task;
            }
        });
    }
    for (auto &thread : threadVector3) {
        thread.join();
    }
    const auto endTime3 = std::chrono::high_resolution_clock::now();
    int totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime3 - startime3).count();
    std::cout << "Runtime: " << totalDuration << "ms" << std::endl;

    assert(answer == 1600);
    assert(totalDuration - expectedDuration3 >= -tolerance3 && totalDuration - expectedDuration3 <= tolerance3);

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

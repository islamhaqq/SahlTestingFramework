#include <cassert>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <atomic>
#include <functional>

static const int ONE_MS_FIB_N = 1136363; // Approximate number of iterations to take 1ms on i9-1200K
struct TestState {
    int total = 0;
    int passed = 0;
};

void testBoolean(bool assertion, TestState &state) {
    state.total += 1;
    state.passed += assertion;
}

std::string getTotalTestCountString(const TestState state) {
    return "Total Tests: " + std::to_string(state.total);
}

bool checkTestStateIsEqual(const TestState testState, const int expectedTestCount, const int expectedPassedTests) {
    return testState.total == expectedTestCount && testState.passed == expectedPassedTests;
}

int getTotalFailedTests(const TestState state) {
    return state.total - state.passed;
}

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    int a = 0, b = 1, c;
    for (int i = 2; i <= n; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

/**
 * To do:
 * Output runtime for each test
 * Output total runtime for all tests
 * Output number of tests
 * Output number of failed tests
 * Output number of passed tests
 * Output error message for failed tests
 * Parallelize tests
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

    // Has correct test count (pass, fail, total)
    assert(checkTestStateIsEqual(testState, 4, 3));
    assert(getTotalFailedTests(testState) == expectedFailedTests);
    assert(getTotalTestCountString(testState) == "Total Tests: 4");

    testBoolean(15 + 100 - 10 == 100 + 5, testState);
    assert(checkTestStateIsEqual(testState, 5, 4));
    assert(getTotalFailedTests(testState) == 1);
    assert(getTotalTestCountString(testState) == "Total Tests: 5");

    testBoolean(95 + 5 == 100, testState);
    assert(checkTestStateIsEqual(testState, 6, 5));
    assert(getTotalFailedTests(testState) == 1);
    assert(getTotalTestCountString(testState) == "Total Tests: 6");


    // ==================== Parallelization ====================


    // No parallelization -- 1 thread -- 4 tasks -- tolerance 40ms
    int expectedDuration = 1000;
    int individualRuntime = 250;
    int fourTasks = 4;
    int tolerance = 40;

    auto starTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < fourTasks; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds (individualRuntime));
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    int finalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - starTime).count();
    std::cout << "Runtime: " << finalDuration << "ms" << std::endl;

    assert(std::abs(finalDuration - expectedDuration) <= tolerance);

    // Static parallelization -- 4 threads -- 4 tasks -- tolerance 25ms
    int expectedDuration2 = 250;
    int individualRuntime2 = 250;
    int tolerance2 = 25;
    int threads4 = 4;

    auto starTime2 = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threadVector;
    threadVector.reserve(threads4);
    for (int i = 0; i < threads4; i++) {
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

    assert(std::abs(finalDuration2 - expectedDuration2) <= tolerance2);

    // Dynamic parallelization -- 16 threads -- 64 tasks -- tolerance 50ms -- sorted queue based
    int expectedDuration3 = 100;
    int tolerance3 = 50;
    int threads16 = 16;

    std::vector<int> tasks = {100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5,
                              100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5,
                              100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5,
                              100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5};
    std::atomic<int> answer;
    answer = 0;
    const auto startTime3 = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threadVector3;
    threadVector3.reserve(threads16);
    std::mutex queueMutex;
    std::queue<int> taskQueue;
    std::sort(tasks.begin(), tasks.end(), std::greater<int>());
    // Queue based task distribution
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
                fibonacci(ONE_MS_FIB_N * task);
                answer += task;
            }
        });
    }
    for (auto &thread : threadVector3) {
        thread.join();
    }
    const auto endTime3 = std::chrono::high_resolution_clock::now();
    int totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime3 - startTime3).count();
    std::cout << "Runtime: " << totalDuration << "ms" << std::endl;

    assert(answer == 1600);
    assert(std::abs(totalDuration - expectedDuration3) <= tolerance3);


    // Dynamic parallelization -- 16 threads -- 64 tasks -- tolerance 15ms -- greedy no queue
    int expectedDuration4 = 100;
    int tolerance4 = 20;

    std::vector<int> tasks4 = {100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5,
                              100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5,
                              100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5,
                              100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5};
    int totalWorkload = 0;
    for (int task : tasks4) {
        totalWorkload += task;
    }
    int optimalThreadWorkload = totalWorkload / threads16;
    std::vector<std::vector<int>> threadTasks;
    threadTasks.reserve(threads16);
    std::sort(tasks4.begin(), tasks4.end(), std::greater<int>());
    for (int i = 0; i < threads16; i++) {
        int workload = 0;
        std::vector<int> threadTask;
        while (workload < optimalThreadWorkload) {
            threadTask.push_back(tasks4.back());
            workload += tasks4.back();
            tasks4.pop_back();
        }
        threadTasks.push_back(threadTask);
    }

    std::atomic<int> answer4;
    answer4 = 0;
    std::vector<std::thread> threadVector4;
    threadVector4.reserve(threads16);
    const auto startTime4 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < threads16; i++) {
        std::vector<int> currentThreadTasks = threadTasks[i];
        threadVector4.emplace_back([currentThreadTasks, &answer4, i]() {
            for (int task : currentThreadTasks) {
                fibonacci(ONE_MS_FIB_N * task);
                answer4 += task;
            }
        });
    }
    for (auto &thread : threadVector4) {
        thread.join();
    }
    const auto endTime4 = std::chrono::high_resolution_clock::now();
    int totalDuration4 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime4 - startTime4).count();
    std::cout << "Runtime: " << totalDuration4 << "ms" << std::endl;
    assert(answer4 == 1600);
    assert(std::abs(totalDuration4 - expectedDuration4) <= tolerance4);

//
//    // Dynamic parallelization -- optimal threads -- 64 function tasks -- tolerance 50ms
//    int expectedDuration4 = 1600;
//    int tolerance4 = 50;
//
//    // function tasks
//    std::vector<int> tasks2 = {100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5,
//                              100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5,
//                              100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5,
//                              100, 50, 25, 0, 10, 0, 50, 25, 50, 50, 0, 25, 0, 5, 5, 5};
//    std::sort(tasks2.begin(), tasks2.end(), std::greater<int>());
//    int taskWorkload = 0;
//    for (int task : tasks2) {
//        taskWorkload += task;
//    }
//    int threads = std::thread::hardware_concurrency();
//    int optimalThreadWorkload = taskWorkload / threads;
//
//    std::queue<std::function<int()>> functionTaskQueue;
//    for (int task : tasks2) {
//        functionTaskQueue.emplace([task]() {
//            std::this_thread::sleep_for(std::chrono::milliseconds(task));
//            return task;
//        });
//    }
//
//    int totalDuration4 = 0;
//    while(!functionTaskQueue.empty())
//    {
//        auto task = functionTaskQueue.front();
//        functionTaskQueue.pop();
//        totalDuration4 += task();
//    }
//    std::cout << "Runtime: " << totalDuration4 << "ms" << std::endl;
//
//    assert(totalDuration4 - expectedDuration4 <= tolerance4);

    // ==================== End ====================

    return 0;
}

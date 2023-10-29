#include "TestingFramework.h"

#include <utility>
#include <string>

TestRegistry& TestRegistry::Instance()
{
    static TestRegistry registry;
    return registry;
}

void TestRegistry::AddTest(ITest* test)
{
    Tests_.emplace_back(test);
}

void TestRegistry::RunAllTests() const
{
    const size_t total = Tests_.size();
    int passed = 0;
    for (const auto& test : Tests_)
    {
        const std::string testName = test->GetName();
        OutputTestPreRun(testName);
            
        test->Run();

        OutputTestPostRun(test->Passed(), testName);

        if (test->Passed())
        {
            passed++;
        }
    }

    OutputTestResults(total, passed);
}

TestRegistry::TestRegistry() = default;

const char* TestRegistry::PluralEnding(const size_t total) const
{
    return total != 1 ? "s" : "";
}

void TestRegistry::OutputTestResults(const size_t total, const int passed) const
{
    const int failed = total - passed;

    std::cout << std::endl << "[==========] " << total << " test" << PluralEnding(total) << " ran." << std::endl;
    std::cout << "[  PASSED  ] " << passed << " test" << PluralEnding(passed) << "." << std::endl;
    std::cout << "[  FAILED  ] " << failed << " test" << PluralEnding(failed) << "." << std::endl;
}

void TestRegistry::OutputTestPreRun(const std::string& testName) const
{
    std::cout << "[  RUN  ] " << testName << std::endl;
}

void TestRegistry::OutputTestPostRun(const bool bPassed, const std::string& testName) const
{
    std::cout << (bPassed ? "[  PASSED  ] " : "[  FAILED  ] ") << testName << std::endl;
}

TestBase::TestBase(std::string  name): Name(std::move(name))
{
    TestRegistry::Instance().AddTest(this);
}

std::string TestBase::GetName()
{
    return Name;
}

bool TestBase::Passed()
{
    return FailureCount == 0;
}

void TestBase::SetShouldLog(const bool bShouldLog)
{
    this->bShouldLog = bShouldLog;
}

void TestBase::ExpectTrue(const bool condition, const std::string& expression, const char* file, int line)
{
    if (!condition)
    {
        FailTest(expression, file, line);
    }
}

void TestBase::ExpectFalse(const bool condition, const std::string& expression, const char* file, int line)
{
    if (condition)
    {
        FailTest(expression, file, line);
    }
}

void TestBase::ExpectStringEquals(const std::string& expected, const std::string& actual, const std::string& expression, const char* file, int line)
{
    if (expected != actual)
    {
        OutputExceptionFailed(expression, file, line);
        // TODO: Refactor.
        if (bShouldLog)
        {
            std::cerr << "Expected: " << expected << " but got " << actual << std::endl;
        }
        FailureCount++;
    }
}

void TestBase::Fail(const char* file, int line)
{
    FailTest("Fail()", file, line);
}

void TestBase::OutputExceptionFailed(const std::string& expression, const char* file, int line) const
{
    if (bShouldLog)
    {
        std::cerr << file << ":" << line << ": Failure: expected " << expression << std::endl;
    }
}

void TestBase::FailTest(const std::string& failureMessage, const std::string& expression, const char* file, int line)
{
    // TODO: Refactor - combine or do something with OutputExceptionFailed.
    if(bShouldLog)
    {
        std::cerr << file << ":" << line << ": Failure: " << failureMessage << std::endl;
    }
    FailTest(expression, file, line);
}

void TestBase::FailTest(const std::string& expression, const char* file, int line)
{
    OutputExceptionFailed(expression, file, line);
    FailureCount++;
}

void TestBase::AssertTrue(bool condition, const std::string &expression, const char *file, int line) {
    if (!condition)
    {
        std::string message = "Assertion failed: (" + expression + ") in file " + file + ", line " + std::to_string(line);
        FailTest(message, expression, file, line);
    }
}

void TestBase::AssertFalse(bool condition, const std::string &expression, const char *file, int line) {
    if (condition)
    {
        std::string message = "Assertion failed: (" + expression + ") in file " + file + ", line " + std::to_string(line);
        FailTest(message, expression, file, line);
    }
}

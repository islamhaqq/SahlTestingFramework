#include "TestingFramework.h"

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
        const string testName = test->GetName();
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

    cout << endl << "[==========] " << total << " test" << PluralEnding(total) << " ran." << endl;
    cout << "[  PASSED  ] " << passed << " test" << PluralEnding(passed) << "." << endl;
    cout << "[  FAILED  ] " << failed << " test" << PluralEnding(failed) << "." << endl;
}

void TestRegistry::OutputTestPreRun(const string testName) const
{
    cout << "[  RUN  ] " << testName << endl;
}

void TestRegistry::OutputTestPostRun(const bool bPassed, const string testName) const
{
    cout << (bPassed ? "[  PASSED  ] " : "[  FAILED  ] ") << testName << endl;
}

TestBase::TestBase(const string& name): Name(name)
{
    TestRegistry::Instance().AddTest(this);
}

string TestBase::GetName()
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

void TestBase::ExpectTrue(const bool condition, const string& expression, const char* file, int line)
{
    if (!condition)
    {
        FailTest(expression, file, line);
    }
}

void TestBase::ExpectFalse(const bool condition, const string& expression, const char* file, int line)
{
    if (condition)
    {
        FailTest(expression, file, line);
    }
}

void TestBase::ExpectStringEquals(const string& expected, const string& actual, const string& expression, const char* file, int line)
{
    if (expected != actual)
    {
        OutputExceptionFailed(expression, file, line);
        // TODO: Refactor.
        if (bShouldLog)
        {
            cerr << "Expected: " << expected << " but got " << actual << endl;
        }
        FailureCount++;
    }
}

void TestBase::Fail(const char* file, int line)
{
    FailTest("Fail()", file, line);
}

void TestBase::OutputExceptionFailed(const string& expression, const char* file, int line) const
{
    if (bShouldLog)
    {
        cerr << file << ":" << line << ": Failure: expected " << expression << endl;
    }
}

void TestBase::FailTest(const string& failureMessage, const string& expression, const char* file, int line)
{
    // TODO: Refactor - combine or do something with OutputExceptionFailed.
    if(bShouldLog)
    {
        cerr << file << ":" << line << ": Failure: " << failureMessage << endl;
    }
    FailTest(expression, file, line);
}

void TestBase::FailTest(const string& expression, const char* file, int line)
{
    OutputExceptionFailed(expression, file, line);
    FailureCount++;
}
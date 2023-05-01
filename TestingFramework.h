// Copyright Syed Aman. All rights reserved.
#pragma once
#include <iostream>
#include <vector>

using namespace std;


#define TEST(TestCaseName, TestName) \
    class TestCaseName##_##TestName : public TestBase { \
    public: \
        TestCaseName##_##TestName() : TestBase(#TestCaseName "." #TestName) {} \
        void Run() override; \
    } TestCaseName##_##TestName##_instance; \
    void TestCaseName##_##TestName::Run()

#define EXPECT_TRUE(condition) \
    ExpectTrue(condition, #condition " == true", __FILE__, __LINE__)

#define EXPECT_FALSE(condition) \
    ExpectFalse(condition, #condition " == false", __FILE__, __LINE__)

#define EXPECT_EQ(expected, actual) \
    ExpectEqual(expected, actual, #expected " == " #actual, __FILE__, __LINE__)

#define EXPECT_NE(expected, actual) \
    ExpectNotEqual(expected, actual, #expected " != " #actual, __FILE__, __LINE__)

#define EXPECT_FAIL(expression) \
    ExpectFailure([&]() { expression; })

#define EXPECT_STREQ(expected, actual) \
    ExpectStringEquals(expected, actual, #expected " == " #actual, __FILE__, __LINE__)

#define EXPECT_LE(left, right) \
    ExpectTrue(left <= right, #left " <= " #right, __FILE__, __LINE__) // TODO: Make a ExpectLE if necessary.

#define FAIL() \
    Fail(__FILE__, __LINE__)

class ITest
{
public:
    virtual ~ITest() = default;
    virtual void Run() = 0;
    virtual string GetName() = 0;
    virtual bool Passed() = 0;
};


class TestRegistry
{
public:
    static TestRegistry& Instance();
    void AddTest(ITest* test);
    void RunAllTests() const;

private:
    TestRegistry();
    vector<ITest*> Tests_;
    
    const char* PluralEnding(const size_t total) const;
    void OutputTestResults(const size_t total, const int passed) const;
    void OutputTestPreRun(const string testName) const;
    void OutputTestPostRun(const bool bPassed, const string testName) const;
};


class TestBase : public ITest
{
public:
    TestBase(const string& name);
    string GetName() override;
    bool Passed() override;
    void SetShouldLog(const bool bShouldLog);

    template <typename Expression>
    void ExpectFailure(Expression expression);

protected:
    void ExpectTrue(const bool condition, const string& expression, const char* file, int line);
    void ExpectFalse(const bool condition, const string& expression, const char* file, int line);

    template <typename T, typename U, typename EqualityComparator = std::equal_to<T>, std::enable_if_t<!std::is_same_v<EqualityComparator, void>, int> = 0>
    void ExpectEqual(T expected, U actual, const string& expression, const char* file, int line);

    template <typename T, typename U, typename EqualityComparator = std::equal_to<T>, std::enable_if_t<std::is_same_v<EqualityComparator, void>, int> = 0>
    void ExpectEqual(T expected, U actual, const string& expression, const char* file, int line);

    template <typename T>
    void ExpectNotEqual(const T expected, const T actual, const string& expression, const char* file, int line);

    void ExpectStringEquals(const string& expected, const string& actual, const string& expression, const char* file, int line);
    void Fail(const char* file, int line);
    
private:
    string Name;
    int FailureCount = 0;
    bool bShouldLog = true;
    
    void OutputExceptionFailed(const string& expression, const char* file, int line) const;
};

template <typename Expression>
void TestBase::ExpectFailure(Expression expression)
{
    // Prevent logging of failures within the expression because it is supposed to fail.
    SetShouldLog(false);

    const unsigned int initialFailureCount = FailureCount;
    expression();
    if (FailureCount > initialFailureCount)
    {
        FailureCount--;
    }
    else
    {
        FailureCount = initialFailureCount + 1;
    }

    SetShouldLog(true);
}

template <typename T, typename U, typename EqualityComparator, std::enable_if_t<!std::is_same_v<EqualityComparator, void>, int>>
void TestBase::ExpectEqual(T expected, U actual, const string& expression, const char* file, int line)
{
    EqualityComparator comparator;
    if (!comparator(expected, actual))
    {
        OutputExceptionFailed(expression, file, line);
        FailureCount++;
    }
}

template <typename T, typename U, typename EqualityComparator, std::enable_if_t<std::is_same_v<EqualityComparator, void>, int>>
void TestBase::ExpectEqual(T expected, U actual, const string& expression, const char* file, int line)
{
    cerr << file << ":" << line << ": Failure: No suitable comparator found for the type '" << typeid(T).name() << "'" << endl;
    FailureCount++;
}

template <typename T>
void TestBase::ExpectNotEqual(const T expected, const T actual, const string& expression, const char* file, int line)
{
    if (expected == actual)
    {
        OutputExceptionFailed(expression, file, line);
        FailureCount++;
    }
}

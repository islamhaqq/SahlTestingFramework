// Copyright Syed Aman. All rights reserved.
#pragma once
#include <iostream>
#include <vector>

#define S_TEST(TestCaseName, TestName) \
    class TestCaseName##_##TestName : public TestBase { \
    public: \
        TestCaseName##_##TestName() : TestBase(#TestCaseName "." #TestName) {} \
        void Run() override; \
    } TestCaseName##_##TestName##_instance; \
    void TestCaseName##_##TestName::Run()

#define S_EXPECT_TRUE(condition) \
    ExpectTrue(condition, #condition " == true", __FILE__, __LINE__)

#define S_ASSERT_TRUE(condition) \
    AssertTrue(condition, #condition " == true", __FILE__, __LINE__)

#define S_ASSERT_FALSE(condition) \
    AssertFalse(condition, #condition " == false", __FILE__, __LINE__)

#define S_EXPECT_FALSE(condition) \
    ExpectFalse(condition, #condition " == false", __FILE__, __LINE__)

#define S_EXPECT_EQ(actual, expected) \
    ExpectEqual(expected, actual, #expected " == " #actual, __FILE__, __LINE__)

#define S_EXPECT_NE(actual, expected) \
    ExpectNotEqual(expected, actual, #expected " != " #actual, __FILE__, __LINE__)

#define S_EXPECT_FAIL(expression) \
    ExpectFailure([&]() { expression; })

#define S_EXPECT_STREQ(actual, expected) \
    ExpectStringEquals(expected, actual, #expected " == " #actual, __FILE__, __LINE__)

#define S_EXPECT_LE(left, right) \
    ExpectTrue(left <= right, #left " <= " #right, __FILE__, __LINE__) // TODO: Make a ExpectLE if necessary.

#define S_EXPECT_THROW(expression, exceptionType) \
    ExpectThrow<exceptionType>([&]() { expression; }, #expression " throws " #exceptionType, __FILE__, __LINE__)

#define S_FAIL() \
    Fail(__FILE__, __LINE__)

class ITest
{
public:
    virtual ~ITest() = default;
    virtual void Run() = 0;
    virtual std::string GetName() = 0;
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
    std::vector<ITest*> Tests_;
    
    const char* PluralEnding(size_t total) const;
    void OutputTestResults(size_t total, int passed) const;
    void OutputTestPreRun(const std::string& testName) const;
    void OutputTestPostRun(bool bPassed, const std::string& testName) const;
};


class TestBase : public ITest
{
public:
    explicit TestBase(std::string  name);
    std::string GetName() override;
        bool Passed() override;
    void SetShouldLog(bool bShouldLog);

    template <typename Expression>
    void ExpectFailure(Expression expression);

protected:
    void ExpectTrue(bool condition, const std::string& expression, const char* file, int line);
    void ExpectFalse(bool condition, const std::string& expression, const char* file, int line);

    void AssertTrue(bool condition, const std::string& expression, const char* file, int line);
    void AssertFalse(bool condition, const std::string& expression, const char* file, int line);

    template <typename T, typename U, typename EqualityComparator = std::equal_to<T>, std::enable_if_t<!std::is_same_v<EqualityComparator, void>, int> = 0>
    void ExpectEqual(T expected, U actual, const std::string& expression, const char* file, int line);

    template <typename T, typename U, typename EqualityComparator = std::equal_to<T>, std::enable_if_t<std::is_same_v<EqualityComparator, void>, int> = 0>
    void ExpectEqual(T expected, U actual, const std::string& expression, const char* file, int line);

    template <typename T>
    void ExpectNotEqual(T expected, T actual, const std::string& expression, const char* file, int line);

    void ExpectStringEquals(const std::string& expected, const std::string& actual, const std::string& expression, const char* file, int line);
    void Fail(const char* file, int line);

    template <typename T, typename F>
    void ExpectThrow(F func, const std::string& expression, const char* file, int line);
private:
    std::string Name;
    int FailureCount = 0;
    bool bShouldLog = true;
    
    void OutputExceptionFailed(const std::string& expression, const char* file, int line) const;
    void FailTest(const std::string& failureMessage, const std::string& expression, const char* file, int line);
    void FailTest(const std::string& expression, const char* file, int line);
};

template <typename Expression>
void TestBase::ExpectFailure(Expression expression)
{
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
void TestBase::ExpectEqual(T expected, U actual, const std::string& expression, const char* file, int line)
{
    EqualityComparator comparator;
    if (!comparator(expected, actual))
    {
        OutputExceptionFailed(expression, file, line);
        if (bShouldLog)
        {
            std::cerr << ": Expected: " << expected << std::endl;
            std::cerr << ": Actual: " << actual << std::endl;
        }
        FailureCount++;
    }
}

template <typename T, typename U, typename EqualityComparator, std::enable_if_t<std::is_same_v<EqualityComparator, void>, int>>
void TestBase::ExpectEqual(T expected, U actual, const std::string& expression, const char* file, int line)
{
    std::cerr << file << ":" << line << ": Failure: No suitable comparator found for the type '" << typeid(T).name() << "'" << std::endl;
    FailureCount++;
}

template <typename T>
void TestBase::ExpectNotEqual(const T expected, const T actual, const std::string& expression, const char* file, int line)
{
    if (expected == actual)
    {
        FailTest(expression, file, line);
    }
}

template <typename T, typename F>
void TestBase::ExpectThrow(F func, const std::string& expression, const char* file, int line)
{
    try
    {
        func();
        FailTest("Expected exception of type '" + std::string(typeid(T).name()) + "but no exception was thrown.", expression, file, line);
    }
    catch (const T&)
    {
        // Pass.
    }
    catch (...)
    {
        FailTest("Expected exception of type '" + std::string(typeid(T).name()) + "but a different exception was thrown.", expression, file, line);
    }
}

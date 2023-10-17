#include "../TestingFramework.h"
#include "../Mock.h"

class SomeInterface
{
public:
    virtual int SomeMethod() = 0;
    virtual int SomeOtherMethod() = 0;
    virtual bool SomeBoolMethod() = 0;
    virtual std::string SomeStringMethod() = 0;
};

class MockImplementation : public SomeInterface
{
    MOCK_METHOD(int, SomeMethod)
    MOCK_METHOD(int, SomeOtherMethod)
    MOCK_METHOD(bool, SomeBoolMethod)
    MOCK_METHOD(std::string, SomeStringMethod)
};

TEST(MOCK_METHOD, NoArguments)
{
    MockImplementation mockImplementation;
    ON_CALL(mockImplementation, SomeMethod).WillByDefault([]() { return 5; });
    EXPECT_EQ(mockImplementation.SomeMethod(), 5);

    ON_CALL(mockImplementation, SomeOtherMethod).WillByDefault([]() { return 6; });
    EXPECT_EQ(mockImplementation.SomeOtherMethod(), 6);

    ON_CALL(mockImplementation, SomeBoolMethod).WillByDefault([]() { return true; });
    EXPECT_TRUE(mockImplementation.SomeBoolMethod());
    ON_CALL(mockImplementation, SomeBoolMethod).WillByDefault([]() { return false; });
    EXPECT_FALSE(mockImplementation.SomeBoolMethod());

    ON_CALL(mockImplementation, SomeStringMethod).WillByDefault([]() { return "Hello"; });
    EXPECT_STREQ(mockImplementation.SomeStringMethod().c_str(), "Hello");
}

class SomeClass
{
    SomeInterface& implementation;
public:
    SomeClass(SomeInterface& implementation) : implementation(implementation) {}
    int DoStuff() { return implementation.SomeMethod() + implementation.SomeOtherMethod(); }
};

TEST(MOCK_METHOD, WithDependencyInjection)
{
    MockImplementation mockImplementation;

    ON_CALL(mockImplementation, SomeMethod).WillByDefault([]() { return 2; });
    EXPECT_EQ(mockImplementation.SomeMethod(), 2);
    ON_CALL(mockImplementation, SomeOtherMethod).WillByDefault([]() { return 1; });
    EXPECT_EQ(mockImplementation.SomeOtherMethod(), 1);

    SomeClass someClass(mockImplementation);
    EXPECT_EQ(someClass.DoStuff(), 3);
}

class SomeInterfaceX
{
public:
    virtual int SomeMethodWithArg(int a) = 0;
    virtual int SomeOtherMethodWithArg(float a) = 0;
    virtual float SomeMethodThatReturnsFloat(float a) = 0;
    virtual int SomeMethodWithTwoArgs(int a, int b) = 0;
};

class MockImplementationX : public SomeInterfaceX
{
    MOCK_METHOD_NEW(int, SomeMethodWithArg, int, a)
    MOCK_METHOD_NEW(int, SomeOtherMethodWithArg, float, a)
    MOCK_METHOD_NEW(float, SomeMethodThatReturnsFloat, float, a)
    MOCK_METHOD_NEW(int, SomeMethodWithTwoArgs, int, a, int, b)
};

TEST(MockMethod, WithMultipleArguments)
{
    MockImplementationX mockImplementationX;
    ON_CALL(mockImplementationX, SomeMethodWithArg).WillByDefault([](int a) { return a; });
    EXPECT_EQ(mockImplementationX.SomeMethodWithArg(5), 5);

    ON_CALL(mockImplementationX, SomeOtherMethodWithArg).WillByDefault([](int a) { return a + 1; });
    EXPECT_EQ(mockImplementationX.SomeOtherMethodWithArg(5), 6);

    ON_CALL(mockImplementationX, SomeMethodThatReturnsFloat).WillByDefault([](float a) { return a + 1.1f; });
    EXPECT_EQ(mockImplementationX.SomeMethodThatReturnsFloat(5.0f), 6.1f);

    ON_CALL(mockImplementationX, SomeMethodWithTwoArgs).WillByDefault([](int a, int b) { return a + b; });
    EXPECT_EQ(mockImplementationX.SomeMethodWithTwoArgs(5, 6), 11);
}


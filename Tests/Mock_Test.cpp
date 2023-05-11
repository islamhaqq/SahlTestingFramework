#include "../TestingFramework.h"
#include "../Mock.h"

class SomeInterface
{
public:
    virtual int SomeMethod() = 0;
    virtual int SomeOtherMethod() = 0;
    virtual bool SomeBoolMethod() = 0;
    virtual std::string SomeStringMethod() = 0;
    virtual int SomeMethodWithArg(int a) = 0;
    // virtual int SomeMethodWithArgs(int a, int b) = 0;
};

class MockImplementation : public SomeInterface
{
    MOCK_METHOD(int, SomeMethod)
    MOCK_METHOD(int, SomeOtherMethod)
    MOCK_METHOD(bool, SomeBoolMethod)
    MOCK_METHOD(std::string, SomeStringMethod)
    MOCK_METHOD(int, SomeMethodWithArg, int)
    // MOCK_METHOD(int, SomeMethodWithArgs, (int a, int b))
};

TEST(TestingFramework, MOCK_METHOD_basic)
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

    ON_CALL(mockImplementation, SomeMethodWithArg).WillByDefault([](int a) { return a; });
    EXPECT_EQ(mockImplementation.SomeMethodWithArg(5), 5);

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

TEST(TestingFramework, MOCK_METHOD)
{
    MockImplementation mockImplementation;

    ON_CALL(mockImplementation, SomeMethod).WillByDefault([]() { return 2; });
    EXPECT_EQ(mockImplementation.SomeMethod(), 2);
    ON_CALL(mockImplementation, SomeOtherMethod).WillByDefault([]() { return 1; });
    EXPECT_EQ(mockImplementation.SomeOtherMethod(), 1);

    SomeClass someClass(mockImplementation);
    EXPECT_EQ(someClass.DoStuff(), 3);
}
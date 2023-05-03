#include "../TestingFramework.h"
#include "../Mock.h"

class SomeInterface
{
public:
    virtual int SomeMethod() = 0;
};

class SomeClass
{
    SomeInterface& implementation;
public:
    SomeClass(SomeInterface& implementation) : implementation(implementation) {}
    int DoStuff() { return implementation.SomeMethod(); }
};

class MockImplementation : public SomeInterface
{
    MOCK_METHOD(SomeMethod)
};

TEST(TestingFramework, MOCK_METHOD)
{
    MockImplementation mockImplementation;
    ON_CALL(mockImplementation, SomeMethod).WillByDefault(5);
    EXPECT_EQ(mockImplementation.SomeMethod(), 5);

    ON_CALL(mockImplementation, SomeMethod).WillByDefault(6);
    EXPECT_EQ(mockImplementation.SomeMethod(), 6);

    ON_CALL(mockImplementation, SomeMethod).WillByDefault(12324);
    EXPECT_EQ(mockImplementation.SomeMethod(), 12324);

    SomeClass someClass(mockImplementation);
    EXPECT_EQ(someClass.DoStuff(), 12324);
}
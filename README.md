# TestingFramework

A unit testing framework for C++ that includes:
* Test running
* Test specification
* Expectations
* Mocking

The heavy usage of macros takes inspiration from Google Test. Although the implementation is different, the interface for creating tests and mocking has similaries to gtest and gmock. 

## Example Usage
You can take a look at the automation tests under the `/Tests` directory for the most up-to-date self-documentation. But here are some test usages if you don't want to do that:

```cpp
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

TEST(MyTestSuite, MyTest)
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
```

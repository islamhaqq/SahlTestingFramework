#pragma once
#include <functional>

template <typename ReturnType>
class MockTing
{
public:
    ReturnType retVal; \
    void WillByDefault(std::function<ReturnType()> func) { retVal = func(); }
};

#define MOCK_METHOD(methodName, type) \
public: \
    class MockTing##methodName : public MockTing<type> {}; \
    MockTing##methodName theMock##methodName; \
    type methodName() override { return theMock##methodName.retVal; } \

#define ON_CALL(mocked_obj, method) \
    mocked_obj.theMock##method
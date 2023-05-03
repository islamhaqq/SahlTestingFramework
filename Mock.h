#pragma once
#include <functional>

#define MOCK_METHOD(methodName) \
public: \
    int retVal = 5; \
    int methodName() override { return retVal; } \
    void WillByDefault(int x) { retVal = x; }

#define ON_CALL(mocked_obj, method) \
    mocked_obj
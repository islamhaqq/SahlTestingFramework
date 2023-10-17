#pragma once
#include <functional>

#include "InternalMacros.h"

template <typename ReturnType, typename... Args>
class MockFunction
{
public:
    std::function<ReturnType(Args...)> func;

    void WillByDefault(std::function<ReturnType(Args...)> new_func) { func = new_func; }

    ReturnType operator()(Args... args)
    {
        return func(std::forward<Args>(args)...);
    }
};

#define MOCK_METHOD(returnType, methodName, ...) \
public: \
    class MockFunction##methodName : public MockFunction<returnType, ##__VA_ARGS__> {}; \
    MockFunction##methodName theMock##methodName; \
    returnType methodName(__VA_ARGS__) override \
    { \
        auto lambda = [this](auto&&... args) -> returnType \
        { \
            return theMock##methodName.func(std::forward<decltype(args)>(args)...); \
        }; \
        return lambda(__VA_ARGS__); \
    }

#define ON_CALL(mocked_obj, method) \
    mocked_obj.theMock##method

#define MOCK_METHOD_NEW(returnType, methodName, ...) \
    MOCK_METHOD_IMPL_INDIRECTION_FOR_COUNT_ARG_PAIRS(returnType, methodName, COUNT_ARG_PAIRS(__VA_ARGS__), __VA_ARGS__)

#define MOCK_METHOD_IMPL_INDIRECTION_FOR_COUNT_ARG_PAIRS(returnType, methodName, numArgs, ...) \
    MOCK_METHOD_IMPL_(returnType, methodName, numArgs, __VA_ARGS__)

#define MOCK_METHOD_IMPL_(returnType, methodName, numArgs, ...) \
    MOCK_METHOD##numArgs(returnType, methodName, GET_FIRST_ARGUMENT(__VA_ARGS__), a)

#define MOCK_METHOD1(returnType, methodName, arg1Type, arg1Name) \
public: \
    MockFunction<returnType, arg1Type> theMock##methodName; \
    returnType methodName(arg1Type arg1Name) override \
    { \
        return theMock##methodName(arg1Name); \
    }

#define MOCK_METHOD2(returnType, methodName, arg1Type, arg1Name) \
public: \
    MockFunction<int, int, int> theMock##methodName; \
    returnType methodName(int a, int b) override \
    { \
        return theMock##methodName(a, b); \
    }
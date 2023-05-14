#pragma once
#include <functional>

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

#define EXPAND(x) x
#define COUNT_ARGS(...) EXPAND(ARGS_AUGMENTER(__VA_ARGS__, 5, 4, 3, 2, 1, 0))
#define ARGS_AUGMENTER(a1, a2, a3, a4, a5, count, ...) count
#define MOCK_METHOD_NEW(returnType, methodName, ...) MOCK_METHOD_IMPL(returnType, methodName, __VA_ARGS__)
#define MOCK_METHOD_IMPL(returnType, methodName, ...) MOCK_METHOD_IMPL_(returnType, methodName, COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)
#define MOCK_METHOD_IMPL_(returnType, methodName, numArgs, ...) MOCK_METHOD_IMPL__(returnType, methodName, numArgs, __VA_ARGS__)
#define MOCK_METHOD_IMPL__(returnType, methodName, numArgs, ...) MOCK_METHOD##numArgs(returnType, methodName, __VA_ARGS__)

#define MOCK_METHOD0(returnType, methodName) \
public: \
    MockFunction<returnType> theMock##methodName; \
    returnType methodName() override \
    { \
        return theMock##methodName(); \
    }

#define MOCK_METHOD1(returnType, methodName, arg1Type, arg1Name) \
public: \
    MockFunction<returnType, arg1Type> theMock##methodName; \
    returnType methodName(arg1Type arg1Name) override \
    { \
        return theMock##methodName(arg1Name); \
    }
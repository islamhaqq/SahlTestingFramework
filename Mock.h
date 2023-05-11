#pragma once
#include <functional>

template <typename ReturnType, typename... Args>
class MockTing
{
public:
    std::function<ReturnType(Args...)> func;

    void WillByDefault(std::function<ReturnType(Args...)> new_func) { func = new_func; }
};

#define MOCK_METHOD(returnType, methodName, ...) \
public: \
    class MockTing##methodName : public MockTing<returnType, ##__VA_ARGS__> {}; \
    MockTing##methodName theMock##methodName; \
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
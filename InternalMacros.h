#pragma once

#define TEST_TOKEN 123
#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)
#define IS_EMPTY(...) CONCAT(TEST_TOKEN, __VA_ARGS__)
#define CHECK_IS_EMPTY(...) (IS_EMPTY(__VA_ARGS__) == TEST_TOKEN)

#define GET_FIRST_ARGUMENT(a1, ...) a1
#define GET_SIXTH_ARGUMENT(a1, a2, a3, a4, a5, count, ...) count

#define EXPAND(x) x
#define COUNT_ARGS(...) \
    (CHECK_IS_EMPTY(__VA_ARGS__) ? 0 : EXPAND(GET_SIXTH_ARGUMENT(__VA_ARGS__, 5, 4, 3, 2, 1, 0)))
#define COUNT_ARG_PAIRS(...) EXPAND(GET_SIXTH_ARGUMENT(__VA_ARGS__, 3, 2, 2, 1, 1, 0))

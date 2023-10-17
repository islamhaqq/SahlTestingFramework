#pragma once

#define TEST_TOKEN_internal 123
#define CONCAT_IMPL_internal(x, y) x##y
#define CONCAT_internal(x, y) CONCAT_IMPL_internal(x, y)
#define IS_EMPTY_internal(...) CONCAT_internal(TEST_TOKEN_internal, __VA_ARGS__)
#define CHECK_IS_EMPTY_internal(...) (IS_EMPTY_internal(__VA_ARGS__) == TEST_TOKEN_internal)

#define EXPAND(x) x
#define GET_SIXTH_ARGUMENT(a1, a2, a3, a4, a5, count, ...) count
#define COUNT_ARGS(...) EXPAND(GET_SIXTH_ARGUMENT(__VA_ARGS__, 5, 4, 3, 2, 1, 0))
#define COUNT_ARG_PAIRS(...) EXPAND(GET_SIXTH_ARGUMENT(__VA_ARGS__, 3, 2, 2, 1, 1, 0))

#define GET_FIRST_ARGUMENT(a1, ...) a1

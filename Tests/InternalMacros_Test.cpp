#include "../InternalMacros.h"
#include "../TestingFramework.h"


TEST(COUNT_ARGS_internal, ReturnsNumberOfArguments)
{
    EXPECT_EQ(COUNT_ARGS(1, 2, 3, 4, 5), 5);
    EXPECT_EQ(COUNT_ARGS(), 0);
}

TEST(COUNT_ARG_PAIRS_internal, ReturnsNumberOfArgumentPairs)
{
    EXPECT_EQ(COUNT_ARG_PAIRS(int, a), 1);
    EXPECT_EQ(COUNT_ARG_PAIRS(int, float), 1);
    EXPECT_EQ(COUNT_ARG_PAIRS(float, arg1, int, arg2), 2);
}

TEST(CONCAT_internal, ConcatenatesLeftWithRight)
{
    EXPECT_EQ(CONCAT_internal(1, 2), 12);
}

TEST(IS_EMPTY_internal, ReturnsTheTestTokenIfEmptyAndConcatenatedOtherwise)
{
    EXPECT_EQ(TEST_TOKEN_internal, 123);
    EXPECT_EQ(IS_EMPTY_internal(), TEST_TOKEN_internal);
    EXPECT_EQ(IS_EMPTY_internal(1), 1231);
}

TEST(CHECK_IS_EMPTY_internal, ReturnsTrueIfEmpty)
{
    EXPECT_EQ(CHECK_IS_EMPTY_internal(), true);
    EXPECT_EQ(CHECK_IS_EMPTY_internal(99), false);
}

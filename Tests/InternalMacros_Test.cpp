#include "../InternalMacros.h"
#include "../TestingFramework.h"


TEST(COUNT_ARGS, ReturnsNumberOfArguments)
{
    EXPECT_EQ(COUNT_ARGS(1, 2, 3, 4, 5), 5);
    EXPECT_EQ(COUNT_ARGS(), 0);
}

TEST(COUNT_ARG_PAIRS, ReturnsNumberOfArgumentPairs)
{
    EXPECT_EQ(COUNT_ARG_PAIRS(int, a), 1);
    EXPECT_EQ(COUNT_ARG_PAIRS(int, float), 1);
    EXPECT_EQ(COUNT_ARG_PAIRS(float, arg1, int, arg2), 2);
}

TEST(CONCAT, ConcatenatesLeftWithRight)
{
    EXPECT_EQ(CONCAT(1, 2), 12);
}

TEST(IS_EMPTY, ReturnsTheTestTokenIfEmptyAndConcatenatedOtherwise)
{
    EXPECT_EQ(TEST_TOKEN, 123);
    EXPECT_EQ(IS_EMPTY(), TEST_TOKEN);
    EXPECT_EQ(IS_EMPTY(1), 1231);
}

TEST(CHECK_IS_EMPTY, ReturnsTrueIfEmpty)
{
    EXPECT_EQ(CHECK_IS_EMPTY(), true);
    EXPECT_EQ(CHECK_IS_EMPTY(99), false);
}

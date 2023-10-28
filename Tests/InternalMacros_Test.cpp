#include "../InternalMacros.h"
#include "../TestingFramework.h"


// S_TEST(COUNT_ARGS, ReturnsNumberOfArguments)
// {
//     S_EXPECT_EQ(COUNT_ARGS(1, 2, 3, 4, 5), 5);
//     S_EXPECT_EQ(COUNT_ARGS(), 0);
// }
//
// S_TEST(COUNT_ARG_PAIRS, ReturnsNumberOfArgumentPairs)
// {
//     S_EXPECT_EQ(COUNT_ARG_PAIRS(int, a), 1);
//     S_EXPECT_EQ(COUNT_ARG_PAIRS(int, float), 1);
//     S_EXPECT_EQ(COUNT_ARG_PAIRS(float, arg1, int, arg2), 2);
// }

S_TEST(CONCAT, ConcatenatesLeftWithRight)
{
    S_EXPECT_STREQ(CONCAT(1, 2), "12");
}

//S_TEST(IS_EMPTY, ReturnsTheTestTokenIfEmptyAndConcatenatedOtherwise)
//{
//    S_EXPECT_EQ(TEST_TOKEN, "123");
//    S_EXPECT_EQ(IS_EMPTY(), TEST_TOKEN);
//    S_EXPECT_EQ(IS_EMPTY(1), "1231");
//}

// S_TEST(CHECK_IS_EMPTY, ReturnsTrueIfEmpty)
// {
//     S_EXPECT_TRUE(CHECK_IS_EMPTY());
//     S_EXPECT_FALSE(CHECK_IS_EMPTY(99));
// }

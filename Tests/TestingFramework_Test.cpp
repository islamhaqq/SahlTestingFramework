#include "../TestingFramework.h"

S_TEST(TestingFramework, ExpectTrue) {
    S_EXPECT_TRUE(true);
    S_EXPECT_FALSE(false);
    S_EXPECT_FAIL(S_EXPECT_TRUE(false));
}

S_TEST(TestingFramework, ExpectFalse) {
    S_EXPECT_FALSE(false);
    S_EXPECT_TRUE(true);
    S_EXPECT_FAIL(S_EXPECT_FALSE(true));
}

S_TEST(TestingFramework, ExpectEqual) {
    int a = 5;
    int b = 5;
    S_EXPECT_EQ(a, b);
    S_EXPECT_FAIL(S_EXPECT_EQ(5, 6));
}

S_TEST(TestingFramework, ExpectNotEqual) {
    int a = 5;
    int b = 6;
    S_EXPECT_NE(a, b);
    S_EXPECT_FAIL(S_EXPECT_NE(5, 5));
}

S_TEST(TestingFramework, S_EXPECT_STREQ)
{
    S_EXPECT_STREQ("Hello", "Hello");
    S_EXPECT_FAIL(S_EXPECT_STREQ("Hello", "World"));
}

S_TEST(TestingFramework, S_FAIL)
{
    S_EXPECT_FAIL(S_FAIL());
}

S_TEST(TestingFramework, S_EXPECT_LE) {
    S_EXPECT_LE(5, 10);
    S_EXPECT_FAIL(S_EXPECT_LE(10, 5));
}

S_TEST(TestingFramework, S_EXPECT_THROW)
{
    const vector<int> empty_vector;
    S_EXPECT_THROW(empty_vector.at(0), std::out_of_range);
    S_EXPECT_FAIL(S_EXPECT_THROW(empty_vector.at(0), std::invalid_argument));

    const vector<int> non_empty_vector{ 1, 2, 3 };
    S_EXPECT_FAIL(S_EXPECT_THROW(non_empty_vector.at(1), std::out_of_range));
}

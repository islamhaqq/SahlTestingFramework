#include "../TestingFramework.h"

TEST(TestingFramework, ExpectTrue) {
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
    EXPECT_FAIL(EXPECT_TRUE(false));
}

TEST(TestingFramework, ExpectFalse) {
    EXPECT_FALSE(false);
    EXPECT_TRUE(true);
    EXPECT_FAIL(EXPECT_FALSE(true));
}

TEST(TestingFramework, ExpectEqual) {
    int a = 5;
    int b = 5;
    EXPECT_EQ(a, b);
    EXPECT_FAIL(EXPECT_EQ(5, 6));
}

TEST(TestingFramework, ExpectNotEqual) {
    int a = 5;
    int b = 6;
    EXPECT_NE(a, b);
    EXPECT_FAIL(EXPECT_NE(5, 5));
}

TEST(TestingFramework, EXPECT_STREQ)
{
    EXPECT_STREQ("Hello", "Hello");
    EXPECT_FAIL(EXPECT_STREQ("Hello", "World"));
}

TEST(TestingFramework, FAIL)
{
    EXPECT_FAIL(FAIL());
}

TEST(TestingFramework, EXPECT_LE) {
    EXPECT_LE(5, 10);
    EXPECT_FAIL(EXPECT_LE(10, 5));
}

TEST(TestingFramework, EXPECT_THROW)
{
    vector<int> empty_vector;
    EXPECT_THROW(empty_vector.at(0), std::out_of_range);

    vector<int> non_empty_vector{ 1, 2, 3 };
    EXPECT_FAIL(EXPECT_THROW(non_empty_vector.at(1), std::out_of_range));
}
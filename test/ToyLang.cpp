#include <gtest/gtest.h>

TEST(ToyLang, Main)
{
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);
}
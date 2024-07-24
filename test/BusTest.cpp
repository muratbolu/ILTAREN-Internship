#include "BusTest.hpp"

#include <gtest/gtest.h>

TEST_F(BusTest, Initialization)
{
}

TEST_F(BusTest, Get)
{
    EXPECT_EQ(b1.getFirst(), 0);
    EXPECT_EQ(b1.getSecond(), 0);
    EXPECT_EQ(b2.getFirst(), 5);
    EXPECT_EQ(b2.getSecond(), 5);
    EXPECT_EQ(b3.getFirst(), 3);
    EXPECT_EQ(b3.getSecond(), 8);
}

TEST_F(BusTest, Alternating)
{
    EXPECT_TRUE(!b1.isAlternating());
    EXPECT_TRUE(!b2.isAlternating());
    EXPECT_TRUE(b3.isAlternating());
}

TEST_F(BusTest, Valid)
{
    EXPECT_TRUE(!b1.isValid());
    EXPECT_TRUE(b2.isValid());
    EXPECT_TRUE(b3.isValid());
}

TEST_F(BusTest, Multiply)
{
    b2 *= 3;
    EXPECT_EQ(b2.getFirst(), 15);
    EXPECT_EQ(b2.getSecond(), 15);

    EXPECT_EQ((b3 * 2).getFirst(), 6);
    EXPECT_EQ((b3 * 2).getSecond(), 16);
}

TEST_F(BusTest, Sum)
{
    EXPECT_EQ(b2.sum(), 10);
    EXPECT_EQ(b3.sum(), 11);
}

TEST_F(BusTest, Equality)
{
    EXPECT_TRUE(b1 == Bus {});
    EXPECT_TRUE(b1 == Bus { 0 });
    EXPECT_TRUE(b2 == Bus { 5 });
    EXPECT_TRUE((b3 == Bus { 3, 8 }));

    EXPECT_FALSE(b1 == Bus { 5 });
    EXPECT_FALSE((b1 == Bus { 3, 8 }));
}

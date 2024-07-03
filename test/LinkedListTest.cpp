#include "LinkedListTest.hpp"

TEST_F(LinkedListTest, Initialization)
{
	;
}

TEST_F(LinkedListTest, Push)
{
	EXPECT_TRUE(ll1.push(12));
	EXPECT_TRUE(ll1.push(15));
	EXPECT_TRUE(ll1.push(18));
	EXPECT_TRUE(ll1.push(21));
	EXPECT_TRUE(ll1.push(24));
	EXPECT_FALSE(ll1.push(27));
}

TEST_F(LinkedListTest, PushLvalue)
{
	int i = 19;
	EXPECT_TRUE(ll1.push(i));
}

TEST_F(LinkedListTest, Size)
{
	EXPECT_EQ(ll1.size(), 0);
	ll1.push(12);
	EXPECT_EQ(ll1.size(), 1);
	ll1.push(15);
	EXPECT_EQ(ll1.size(), 2);
	ll1.push(18);
	EXPECT_EQ(ll1.size(), 3);
	ll1.push(21);
	EXPECT_EQ(ll1.size(), 4);
	ll1.push(24);
	EXPECT_EQ(ll1.size(), 5);
	ll1.push(27);
	EXPECT_EQ(ll1.size(), 5);
}

TEST_F(LinkedListTest, Pop)
{
	ASSERT_TRUE(ll1.push(12));
	EXPECT_EQ(ll1.pop(), 12);
}

TEST_F(LinkedListTest, MultiplePop)
{
	ASSERT_TRUE(ll1.push(12));
	ASSERT_TRUE(ll1.push(15));
	EXPECT_EQ(ll1.pop(), 15);
	EXPECT_EQ(ll1.pop(), 12);
}

TEST_F(LinkedListTest, EmptyPop)
{
	EXPECT_EQ(ll1.pop(), 0);
}

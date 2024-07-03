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
	EXPECT_EQ(ll1.pop(), i);
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

TEST_F(LinkedListTest, ExhaustPool)
{
	ASSERT_TRUE(ll1.push(12));
	ASSERT_TRUE(ll1.push(15));
	ASSERT_TRUE(ll1.push(18));
	ASSERT_TRUE(ll2.push(21));
	ASSERT_TRUE(ll2.push(24));
	EXPECT_FALSE(ll1.push(27));
	EXPECT_FALSE(ll2.push(30));
	ASSERT_EQ(ll1.pop(), 18);
	EXPECT_TRUE(ll2.push(33));
}

TEST_F(LinkedListTest, BigPool)
{
	for (unsigned i{ 0 }; i < 1000; ++i)
	{
		EXPECT_TRUE(ll3.push(i));
	}
}

TEST_F(LinkedListTest, DeallocateList)
{
	{
		LinkedList<int> ll4{ pool };
		ASSERT_TRUE(ll4.push(12));
		ASSERT_TRUE(ll4.push(15));
		ASSERT_TRUE(ll4.push(18));
		ASSERT_TRUE(ll1.push(21));
		ASSERT_TRUE(ll2.push(24));
	}
	EXPECT_TRUE(ll1.push(27));
	EXPECT_TRUE(ll2.push(30));
	EXPECT_TRUE(ll2.push(33));
	EXPECT_EQ(ll1.size(), 2);
	EXPECT_EQ(ll2.size(), 3);
}

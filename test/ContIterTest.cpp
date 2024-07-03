#include "ContIterTest.hpp"

TEST_F(ContIterTest, Initialization)
{
	EXPECT_EQ(it1, it2);
	EXPECT_EQ(*it1, *it2);
	EXPECT_EQ(*it1, 5);
}

TEST_F(ContIterTest, Assignment)
{
	*it1 = 6;
	EXPECT_EQ(*it1, 6);
	EXPECT_EQ(*it2, 6); // due to same pointer
}

TEST_F(ContIterTest, PreIncrement)
{
	ContIter<int> it{ p2 };
	EXPECT_EQ(it, it3);
	++it3;
	EXPECT_FALSE(it == it3);
}

TEST_F(ContIterTest, PostIncrement)
{
	ContIter<int> it{ p2 };
	EXPECT_EQ(it, it3);
	it3++;
	EXPECT_FALSE(it == it3);
}

TEST_F(ContIterTest, PreDecrement)
{
	ContIter<int> it{ p2 + 1 };
	EXPECT_FALSE(it == it3);
	--it;
	EXPECT_EQ(it, it3);
}

TEST_F(ContIterTest, PostDecrement)
{
	ContIter<int> it{ p2 + 1 };
	EXPECT_FALSE(it == it3);
	it--;
	EXPECT_EQ(it, it3);
}

TEST_F(ContIterTest, Iterate)
{
	ContIter<int> it{ p2 };
	for (unsigned i{ 0 }; i < 4; ++i, ++it3)
	{
		*it3 = i;
	}
	for (unsigned i{ 0 }; i < 4; ++i, ++it)
	{
		EXPECT_EQ(*it, i);
	}
}

TEST_F(ContIterTest, Comparison)
{
	EXPECT_TRUE(it1 == it2);
	*it1 = 6;
	EXPECT_TRUE(*it1 == *it2);
	EXPECT_TRUE(it1 == it2); // due to same pointer
}

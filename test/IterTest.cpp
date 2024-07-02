#include "IterTest.hpp"

TEST_F(IterTest, Initialization)
{
	EXPECT_EQ(it1, it2);
	EXPECT_EQ(*it1, *it2);
	EXPECT_EQ(*it1, 5);
}

TEST_F(IterTest, Assignment)
{
	*it1 = 6;
	EXPECT_EQ(*it1, 6);
	EXPECT_EQ(*it2, 6); // due to same pointer
}

TEST_F(IterTest, PreIncrement)
{
	Iter<int> it{ p2 };
	EXPECT_EQ(it, it3);
	++it3;
	EXPECT_FALSE(it == it3);
}

TEST_F(IterTest, PostIncrement)
{
	Iter<int> it{ p2 };
	EXPECT_EQ(it, it3);
	it3++;
	EXPECT_FALSE(it == it3);
}

TEST_F(IterTest, PreDecrement)
{
	Iter<int> it{ p2 + 1 };
	EXPECT_FALSE(it == it3);
	--it;
	EXPECT_EQ(it, it3);
}

TEST_F(IterTest, PostDecrement)
{
	Iter<int> it{ p2 + 1 };
	EXPECT_FALSE(it == it3);
	it--;
	EXPECT_EQ(it, it3);
}

TEST_F(IterTest, Iterate)
{
	Iter<int> it{ p2 };
	for (unsigned i{ 0 }; i < 4; ++i, ++it3)
	{
		*it3 = i;
	}
	for (unsigned i{ 0 }; i < 4; ++i, ++it)
	{
		EXPECT_EQ(*it, i);
	}
}

TEST_F(IterTest, Comparison)
{
	EXPECT_TRUE(it1 == it2);
	*it1 = 6;
	EXPECT_TRUE(*it1 == *it2);
	EXPECT_TRUE(it1 == it2); // due to same pointer
}

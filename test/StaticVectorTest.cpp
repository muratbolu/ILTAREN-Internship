#include "StaticVectorTest.hpp"

TEST_F(StaticVectorTest, Initialization)
{
	EXPECT_EQ(vec1.size(), 3);
	EXPECT_FALSE(vec1.empty());
}

TEST_F(StaticVectorTest, At1)
{
	vec0.at(0) = 'H';
	vec0.at(1) = 'i';
	EXPECT_EQ(vec0.at(0), 'H');
	EXPECT_EQ(vec0.at(1), 'i');
}

TEST_F(StaticVectorTest, At2)
{
	EXPECT_EQ(vec1.at(0), 3);
	EXPECT_EQ(vec1.at(1), 5);
	EXPECT_EQ(vec1.at(2), 7);
}

TEST_F(StaticVectorTest, Assignment1)
{
	vec0[0] = 'A';
	vec0[1] = 'B';
	EXPECT_EQ(vec0[0], 'A');
	EXPECT_EQ(vec0[1], 'B');
}

TEST_F(StaticVectorTest, Assignment2)
{
	vec1.front() = 9;
	vec1.back() = 11;
	EXPECT_EQ(vec1[0], 9);
	EXPECT_EQ(vec1[1], 5);
	EXPECT_EQ(vec1[2], 11);
}

TEST_F(StaticVectorTest, Assignment3)
{
	EXPECT_EQ(vec2.front().front(), 12);
	EXPECT_EQ(vec2.back().back(), 15);
}

TEST_F(StaticVectorTest, Iterate1)
{
	vec1.clear();
	for (int it : vec1)
	{
		EXPECT_EQ(it, 0);
	}
	for (ContIter<int> it{ vec1.begin() }; it != vec1.end(); ++it)
	{
		EXPECT_EQ(*it, 0);
	}
	for (ContIter<int> it{ vec1.begin() }; it != vec1.end(); it++)
	{
		EXPECT_EQ(*it, 0);
	}
	for (ContIter<int> it{ vec1.end() }; it-- != vec1.begin();)
	{
		EXPECT_EQ(*it, 0);
	}
}

TEST_F(StaticVectorTest, Iterate2)
{
	for (StaticVector<unsigned, 4>&it2 : vec2)
	{
		for (unsigned& it1 : it2)
		{
			it1 = 1;
		}
	}
	for (unsigned i = 0; i < vec2.size(); ++i)
	{
		for (unsigned j = 0; j < vec2[i].size(); ++j)
		{
			EXPECT_EQ(vec2[i][j], 1);
		}
	}
}

TEST_F(StaticVectorTest, Empty)
{
	EXPECT_EQ(vec1.empty(), false);
}

TEST_F(StaticVectorTest, Size)
{
	EXPECT_EQ(vec1.size(), 3);
}

TEST_F(StaticVectorTest, Clear)
{
	vec1.clear();
	for (ContIter<int> it{ vec1.begin() }; it != vec1.end(); ++it)
	{
		EXPECT_EQ(*it, 0);
	}
}

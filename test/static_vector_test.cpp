#include <gtest/gtest.h>

#include "static_vector.tpp"

class StaticVectorTest : public testing::Test
{
protected:
	StaticVectorTest()
	{
		vec1[0] = 3;
		vec1[1] = 5;
		vec1[2] = 7;
	}
	static_vector<int, 3> vec1;
};

TEST_F(StaticVectorTest, Initialization) {
	EXPECT_EQ(vec1.size(), 3);
	EXPECT_EQ(vec1.empty(), false);
}

TEST_F(StaticVectorTest, Assignment) {
	EXPECT_EQ(vec1[0], 3);
	EXPECT_EQ(vec1[1], 5);
	EXPECT_EQ(vec1[2], 7);
}

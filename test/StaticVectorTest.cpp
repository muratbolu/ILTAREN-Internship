#include "StaticVectorTest.hpp"

TEST_F(StaticVectorTest, Initialization) {
	EXPECT_EQ(vec1.size(), 3);
	EXPECT_EQ(vec1.empty(), false);
}

TEST_F(StaticVectorTest, Assignment) {
	EXPECT_EQ(vec1[0], 3);
	EXPECT_EQ(vec1[1], 5);
	EXPECT_EQ(vec1[2], 7);
}

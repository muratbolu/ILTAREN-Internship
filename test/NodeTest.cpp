#include "NodeTest.hpp"

TEST_F(NodeTest, Initialization)
{
	EXPECT_EQ(n1.data(), 5);
	EXPECT_EQ(n2.data(), 7);
	EXPECT_EQ(n3.data(), 9);
}

TEST_F(NodeTest, Data)
{
	n1.data() = 11;
	EXPECT_EQ(n1.data(), 11);
	n2.data() = n1.data();
	EXPECT_EQ(n2.data(), n1.data());
}

TEST_F(NodeTest, Linkage)
{
	n1.next() = &n2;
	EXPECT_EQ(n1.next(), &n2);

	n1.next()->prev() = &n1;
	EXPECT_EQ(n1.next()->prev(), &n1);
}

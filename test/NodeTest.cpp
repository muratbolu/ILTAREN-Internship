#include "NodeTest.hpp"

TEST_F(NodeTest, Initialization)
{
	;
}

TEST_F(NodeTest, Data)
{
	n1.data() = new int{ 5 };
	EXPECT_EQ(*n1.data(), 5);
}

TEST_F(NodeTest, Linkage)
{
	n1.next() = &n2;
	EXPECT_EQ(n1.next(), &n2);

	n1.next()->prev() = &n1;
	EXPECT_EQ(n1.next()->prev(), &n1);
}

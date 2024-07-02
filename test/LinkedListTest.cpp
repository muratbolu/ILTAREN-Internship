#include "LinkedListTest.hpp"

TEST_F(LinkedListTest, Initialization)
{
	;
}

TEST_F(LinkedListTest, Traversal)
{
	Node<int>* curr = ll.head();
	ASSERT_TRUE(curr != nullptr);
	for (unsigned i{ 0 }; i < 5; ++i)
	{
		curr = curr->next();
		ASSERT_TRUE(curr != nullptr);
	}
}

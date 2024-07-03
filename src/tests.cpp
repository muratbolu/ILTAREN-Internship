#include <gtest/gtest.h>

#include "ContIterTest.hpp"
#include "LinkedListTest.hpp"
#include "NodeIterTest.hpp"
#include "NodeTest.hpp"
#include "ObjectPoolTest.hpp"
#include "StaticVectorTest.hpp"

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

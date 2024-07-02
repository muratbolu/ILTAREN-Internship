#include <gtest/gtest.h>

#include "IterTest.hpp"
#include "StaticVectorTest.hpp"

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

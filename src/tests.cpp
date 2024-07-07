#include <gtest/gtest.h>

// Apparently no need to include test files here!

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include "ObjectPoolTest.hpp"

TEST_F(ObjectPoolTest, Initialization)
{
}

TEST_F(ObjectPoolTest, Allocation)
{
    int* ptr1 = p1.allocate();
    EXPECT_TRUE(ptr1 != nullptr);
    int* ptr2 = p1.allocate();
    EXPECT_TRUE(ptr2 != nullptr);

    EXPECT_TRUE(ptr1 != ptr2);
}

TEST_F(ObjectPoolTest, TooManyAllocations)
{
    int* ptr1 = p1.allocate();
    ASSERT_TRUE(ptr1 != nullptr);
    int* ptr2 = p1.allocate();
    ASSERT_TRUE(ptr2 != nullptr);

    int* ptr3 = p1.allocate();
    EXPECT_TRUE(ptr3 == nullptr);
}

TEST_F(ObjectPoolTest, Deallocation)
{
    int* ptr1 = p1.allocate();
    ASSERT_TRUE(ptr1 != nullptr);
    EXPECT_TRUE(p1.deallocate(ptr1));
}

TEST_F(ObjectPoolTest, NullDeallocation)
{
    EXPECT_FALSE(p1.deallocate(nullptr));
}

TEST_F(ObjectPoolTest, UnderRangeDeallocation)
{
    int* ptr = p1.allocate();
    EXPECT_FALSE(p1.deallocate(ptr - 1));
}

TEST_F(ObjectPoolTest, OverRangeDeallocation)
{
    int* ptr = p1.allocate();
    EXPECT_FALSE(p1.deallocate(ptr + 2));
}

TEST_F(ObjectPoolTest, VacantDeallocation)
{
    int* ptr = p1.allocate();
    EXPECT_FALSE(p1.deallocate(ptr + 1));
}

TEST_F(ObjectPoolTest, AllocateAndDeallocate)
{
    int* ptr1 = p1.allocate();
    int* ptr2 = p1.allocate();
    p1.deallocate(ptr1);
    int* ptr3 = p1.allocate();
    EXPECT_EQ(ptr1, ptr3);
}

TEST_F(ObjectPoolTest, AllocateAll)
{
    for (unsigned i { 0 }; i < 100; ++i)
    {
        p2.allocate();
    }
}

TEST_F(ObjectPoolTest, AllocateAndDeallocateImmediately)
{
    p1.deallocate(p1.allocate());
}

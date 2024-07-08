#include "LinkedListTest.hpp"

#include <utility>

TEST_F(LinkedListTest, Initialization)
{
}

TEST_F(LinkedListTest, CopyConstructor)
{
    ASSERT_TRUE(ll1.push(5));
    ASSERT_TRUE(ll1.push(7));
    LinkedList<int> ll { ll1 };
    EXPECT_EQ(*ll[0], 5);
    EXPECT_EQ(*ll[1], 7);
    for (unsigned i { 0 }; i < ll.size(); ++i)
    {
        EXPECT_EQ(*ll[i], *ll1[i]);
        EXPECT_FALSE(ll[i] == ll1[i]);
    }
}

TEST_F(LinkedListTest, MoveConstructor)
{
    ASSERT_TRUE(ll1.push(5));
    ASSERT_TRUE(ll1.push(7));
    int* ptr0 = ll1[0];
    int* ptr1 = ll1[1];
    LinkedList<int> ll { std::move(ll1) };
    EXPECT_EQ(*ll[0], 5);
    EXPECT_EQ(*ll[1], 7);
    EXPECT_EQ(ll[0], ptr0);
    EXPECT_EQ(ll[1], ptr1);
}

TEST_F(LinkedListTest, CopyAssignment)
{
    ASSERT_TRUE(ll1.push(5));
    ASSERT_TRUE(ll1.push(7));
    ASSERT_TRUE(ll2.push(9));
    ASSERT_TRUE(ll2.push(11));
    ll1 = ll2;
    EXPECT_EQ(*ll1[0], 9);
    EXPECT_EQ(*ll1[1], 11);
    EXPECT_FALSE(ll1[0] == ll2[0]);
    EXPECT_FALSE(ll1[1] == ll2[1]);
}

TEST_F(LinkedListTest, MoveAssignment)
{
    ASSERT_TRUE(ll1.push(5));
    ASSERT_TRUE(ll1.push(7));
    ASSERT_TRUE(ll2.push(9));
    ASSERT_TRUE(ll2.push(11));
    int* ptr0 { ll2[0] };
    int* ptr1 { ll2[1] };
    ll1 = std::move(ll2);
    EXPECT_EQ(*ll1[0], 9);
    EXPECT_EQ(*ll1[1], 11);
    EXPECT_EQ(ll1[0], ptr0);
    EXPECT_EQ(ll1[1], ptr1);
}

TEST_F(LinkedListTest, Push)
{
    EXPECT_TRUE(ll1.push(12));
    EXPECT_TRUE(ll1.push(15));
    EXPECT_TRUE(ll1.push(18));
    EXPECT_TRUE(ll1.push(21));
    EXPECT_TRUE(ll1.push(24));
    EXPECT_FALSE(ll1.push(27));
}

TEST_F(LinkedListTest, PushLvalue)
{
    int i = 19;
    EXPECT_TRUE(ll1.push(i));
    EXPECT_EQ(*ll1.pop(), i);
}

TEST_F(LinkedListTest, Size)
{
    EXPECT_EQ(ll1.size(), 0);
    ll1.push(12);
    EXPECT_EQ(ll1.size(), 1);
    ll1.push(15);
    EXPECT_EQ(ll1.size(), 2);
    ll1.push(18);
    EXPECT_EQ(ll1.size(), 3);
    ll1.push(21);
    EXPECT_EQ(ll1.size(), 4);
    ll1.push(24);
    EXPECT_EQ(ll1.size(), 5);
    ll1.push(27);
    EXPECT_EQ(ll1.size(), 5);
}

TEST_F(LinkedListTest, Pop)
{
    ASSERT_TRUE(ll1.push(12));
    EXPECT_EQ(*ll1.pop(), 12);
}

TEST_F(LinkedListTest, MultiplePop)
{
    ASSERT_TRUE(ll1.push(12));
    ASSERT_TRUE(ll1.push(15));
    EXPECT_EQ(*ll1.pop(), 15);
    EXPECT_EQ(*ll1.pop(), 12);
}

TEST_F(LinkedListTest, EmptyPop)
{
    EXPECT_EQ(ll1.pop(), nullptr);
}

TEST_F(LinkedListTest, ExhaustPool)
{
    ASSERT_TRUE(ll1.push(12));
    ASSERT_TRUE(ll1.push(15));
    ASSERT_TRUE(ll1.push(18));
    ASSERT_TRUE(ll2.push(21));
    ASSERT_TRUE(ll2.push(24));
    EXPECT_FALSE(ll1.push(27));
    EXPECT_FALSE(ll2.push(30));
    ASSERT_EQ(*ll1.pop(), 18);
    EXPECT_TRUE(ll2.push(33));
}

TEST_F(LinkedListTest, BigPool)
{
    for (unsigned i { 0 }; i < 1000; ++i)
    {
        EXPECT_TRUE(ll3.push(i));
    }
}

TEST_F(LinkedListTest, DeallocateList)
{
    {
        LinkedList<int> ll4;
        ll4.pool() = &pool;
        ASSERT_TRUE(ll4.push(12));
        ASSERT_TRUE(ll4.push(15));
        ASSERT_TRUE(ll4.push(18));
        ASSERT_TRUE(ll1.push(21));
        ASSERT_TRUE(ll2.push(24));
    }
    EXPECT_TRUE(ll1.push(27));
    EXPECT_TRUE(ll2.push(30));
    EXPECT_TRUE(ll2.push(33));
    EXPECT_EQ(ll1.size(), 2);
    EXPECT_EQ(ll2.size(), 3);
}

TEST_F(LinkedListTest, At1)
{
    ASSERT_TRUE(ll1.push(12));
    EXPECT_EQ(*ll1.at(0), 12);
}

TEST_F(LinkedListTest, At2)
{
    ASSERT_TRUE(ll1.push(12));
    ASSERT_TRUE(ll1.push(15));
    ASSERT_TRUE(ll1.push(18));
    EXPECT_EQ(*ll1.at(0), 12);
    EXPECT_EQ(*ll1.at(1), 15);
    EXPECT_EQ(*ll1.at(2), 18);
}

TEST_F(LinkedListTest, At3)
{
    ASSERT_TRUE(ll1.push(12));
    EXPECT_EQ(ll1.at(1), nullptr);
}

TEST_F(LinkedListTest, Bracket1)
{
    ASSERT_TRUE(ll1.push(12));
    EXPECT_EQ(*ll1[0], 12);
}

TEST_F(LinkedListTest, Bracket2)
{
    ASSERT_TRUE(ll1.push(12));
    ASSERT_TRUE(ll1.push(15));
    ASSERT_TRUE(ll1.push(18));
    EXPECT_EQ(*ll1[0], 12);
    EXPECT_EQ(*ll1[1], 15);
    EXPECT_EQ(*ll1[2], 18);
}

TEST_F(LinkedListTest, Bracket3)
{
    ASSERT_TRUE(ll1.push(12));
    EXPECT_EQ(ll1[1], nullptr);
}

TEST_F(LinkedListTest, Write)
{
    ASSERT_TRUE(ll1.push(12));
    *ll1[0] = 15;
    EXPECT_EQ(*ll1[0], 15);
}

TEST_F(LinkedListTest, InvalidAccess1)
{
    ASSERT_TRUE(ll1.push(12));
    EXPECT_EQ(ll1[1], nullptr);
}

TEST_F(LinkedListTest, InvalidAccess2)
{
    EXPECT_EQ(ll1[0], nullptr);
}

TEST_F(LinkedListTest, InvalidAccess3)
{
    ASSERT_TRUE(ll1.push(12));
    EXPECT_EQ(ll1[-1], nullptr);
}

TEST_F(LinkedListTest, Front)
{
    ASSERT_TRUE(ll1.push(12));
    EXPECT_EQ(*ll1.front(), 12);
}

TEST_F(LinkedListTest, Back)
{
    ASSERT_TRUE(ll1.push(12));
    ASSERT_TRUE(ll1.push(15));
    ASSERT_TRUE(ll1.push(18));
    EXPECT_EQ(*ll1.back(), 18);
}

TEST_F(LinkedListTest, Print)
{
    ASSERT_TRUE(ll1.push(12));
    ASSERT_TRUE(ll1.push(15));
    ASSERT_TRUE(ll1.push(18));
    // TODO: figure out a way to test print
}

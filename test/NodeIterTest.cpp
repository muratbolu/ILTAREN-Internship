#include "NodeIterTest.hpp"

#include "iter/NodeIter.tpp"

#include <gtest/gtest.h>

TEST_F(NodeIterTest, Initialization)
{
    ASSERT_TRUE(ll.pushBack(1));
    NodeIter<int> it1 { ll.begin() };
    NodeIter<int> it2 { ll.begin() };
    EXPECT_EQ(it1, it2);
}

TEST_F(NodeIterTest, Assignment)
{
    ASSERT_TRUE(ll.pushBack(3));
    ASSERT_TRUE(ll.pushBack(5));
    NodeIter<int> it1 { ll.begin() };
    *it1 = 7;
    it1++;
    *it1 = 9;
    NodeIter<int> it2 { ll.begin() };
    EXPECT_EQ(*it2, 7);
    ++it2;
    EXPECT_EQ(*it2, 9);
}

TEST_F(NodeIterTest, Traversal1)
{
    ASSERT_TRUE(ll.pushBack(1));
    ASSERT_TRUE(ll.pushBack(1));
    for (int i : ll)
    {
        EXPECT_EQ(i, 1);
    }
}

TEST_F(NodeIterTest, Traversal2)
{
    ASSERT_TRUE(ll.pushBack(3));
    ASSERT_TRUE(ll.pushBack(5));
    NodeIter<int> it { ll.begin() };
    EXPECT_EQ(*it, 3);
    it++;
    EXPECT_EQ(*it, 5);
}

TEST_F(NodeIterTest, Traversal3)
{
    ASSERT_TRUE(ll.pushBack(2));
    ASSERT_TRUE(ll.pushBack(3));
    ASSERT_TRUE(ll.pushBack(5));
    for (int& i : ll)
    {
        i *= 2;
    }
    NodeIter<int> it { ll.begin() };
    EXPECT_EQ(*it, 4);
    EXPECT_EQ(*++it, 6);
    EXPECT_EQ(*++it, 10);
}

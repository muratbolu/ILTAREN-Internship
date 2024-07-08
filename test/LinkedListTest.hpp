#pragma once

#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"

#include <gtest/gtest.h>

class LinkedListTest : public testing::Test
{
protected:
    LinkedListTest() noexcept
    {
        ll1.pool() = &pool;
        ll2.pool() = &pool;
        ll3.pool() = &pool2;
    }

    ObjectPool<Node<int>, 5> pool;
    LinkedList<int> ll1;
    LinkedList<int> ll2;
    ObjectPool<Node<long>, 1000> pool2;   // 8 KB
    LinkedList<long> ll3;
};

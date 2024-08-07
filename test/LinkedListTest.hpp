#pragma once

#include "sds/LinkedList.tpp"
#include "sds/Node.tpp"
#include "sds/ObjectPool.tpp"

#include <gtest/gtest.h>

class LinkedListTest : public testing::Test
{
protected:
    LinkedListTest() noexcept :
        ll1 { &pool },
        ll2 { &pool },
        ll3 { &pool2 }
    {
    }

    ObjectPool<Node<int>, 6> pool;
    LinkedList<int> ll1;
    LinkedList<int> ll2;
    ObjectPool<Node<long>, 1000> pool2;   // 8 KB
    LinkedList<long> ll3;
};

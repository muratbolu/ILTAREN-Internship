#pragma once

#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"

#include <gtest/gtest.h>

class NodeIterTest : public testing::Test
{
protected:
    NodeIterTest() noexcept
    {
        ll.pool() = &pool;
    }

    ObjectPool<Node<int>, 4> pool;
    LinkedList<int> ll;
};

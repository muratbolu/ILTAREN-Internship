#pragma once

#include "sds/LinkedList.tpp"
#include "sds/Node.tpp"
#include "sds/ObjectPool.tpp"

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

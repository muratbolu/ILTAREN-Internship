#pragma once

#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"

#include <gtest/gtest.h>

class NodeIterTest : public testing::Test
{
protected:
    NodeIterTest() = default;
    ObjectPool<Node<int>, 4> pool;
    LinkedList<int> ll { &pool };
};

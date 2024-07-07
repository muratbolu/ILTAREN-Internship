#pragma once

#include <gtest/gtest.h>

#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"

class NodeIterTest : public testing::Test
{
protected:
    NodeIterTest() = default;
    ObjectPool<Node<int>, 4> pool;
    LinkedList<int> ll{ &pool };
};

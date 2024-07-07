#pragma once

#include <gtest/gtest.h>

#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"

class LinkedListTest : public testing::Test
{
protected:
    LinkedListTest() = default;
    ObjectPool<Node<int>, 5> pool;
    LinkedList<int> ll1{ &pool };
    LinkedList<int> ll2{ &pool };
    ObjectPool<Node<long>, 1000> pool2; // 8 KB
    LinkedList<long> ll3{ &pool2 };
};

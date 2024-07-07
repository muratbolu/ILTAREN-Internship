#pragma once

#include <gtest/gtest.h>

#include "Node.tpp"

class NodeTest : public testing::Test
{
protected:
    NodeTest() = default;
    Node<int> n1;
    Node<int> n2{ 7 };
    Node<int> n3{ 9 };
};

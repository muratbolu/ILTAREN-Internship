#pragma once

#include <gtest/gtest.h>

#include "Node.tpp"

class NodeTest : public testing::Test
{
protected:
	NodeTest() {}
	Node<int> n1{ 5 };
	Node<int> n2{ 7 };
	Node<int> n3{ 9 };
};

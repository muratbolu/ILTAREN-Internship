#pragma once

#include <gtest/gtest.h>

#include "LinkedList.tpp"

class LinkedListTest : public testing::Test
{
protected:
	LinkedListTest() {}
	LinkedList<int, 5> ll;
};

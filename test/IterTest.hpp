#pragma once

#include <gtest/gtest.h>

#include "Iter.tpp"

class IterTest : public testing::Test
{
protected:
	IterTest()
	{
		*it1 = 5;
	}
	int* p1{ new int };
	Iter<int> it1{ p1 };
};

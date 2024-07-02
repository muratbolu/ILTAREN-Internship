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
	Iter<int> it2{ p1 };

	int p2[4];
	Iter<int> it3{ p2 };
};

#pragma once

#include <gtest/gtest.h>

#include "ObjectPool.tpp"

class ObjectPoolTest : public testing::Test
{
protected:
	ObjectPoolTest()
	{
		p1.allocate(2);
	}
	ObjectPool<int, 4> p1;
};

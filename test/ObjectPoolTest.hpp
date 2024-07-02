#pragma once

#include <gtest/gtest.h>

#include "ObjectPool.tpp"

class ObjectPoolTest : public testing::Test
{
protected:
	ObjectPoolTest() {}
	ObjectPool<int, 2> p1;
	ObjectPool<StaticVector<unsigned, 5>, 100> p2;
};

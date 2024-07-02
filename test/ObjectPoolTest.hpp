#pragma once

#include <gtest/gtest.h>

#include "ObjectPool.tpp"

class ObjectPoolTest : public testing::Test
{
protected:
	ObjectPoolTest() {}
	ObjectPool<int, 2> p1;
};

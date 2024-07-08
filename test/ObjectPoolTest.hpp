#pragma once

#include "ObjectPool.tpp"
#include "StaticVector.tpp"

#include <gtest/gtest.h>

class ObjectPoolTest : public testing::Test
{
protected:
    ObjectPoolTest() = default;
    ObjectPool<int, 2> p1;
    ObjectPool<StaticVector<unsigned, 5>, 100> p2;
};

#pragma once

#include "sds/ObjectPool.tpp"
#include "sds/StaticVector.tpp"

#include <gtest/gtest.h>

class ObjectPoolTest : public testing::Test
{
protected:
    ObjectPoolTest() = default;
    ObjectPool<int, 2> p1;
    ObjectPool<StaticVector<unsigned, 5>, 100> p2;
};

#include "MaybeTest.hpp"

#include "sds/LinkedList.tpp"
#include "sds/Node.tpp"
#include "sds/ObjectPool.tpp"
#include "util/Maybe.tpp"

#include <gtest/gtest.h>
#include <memory>
#include <utility>

TEST_F(MaybeTest, Initialization)
{
    nothing<int>();
    just(5);
}

TEST_F(MaybeTest, TwoMaybes)
{
    just(5);
    just(6);
}

TEST_F(MaybeTest, Exists)
{
    Maybe<int> m1 { just(5) };
    ASSERT_TRUE(m1.exists());
}

TEST_F(MaybeTest, Nothing)
{
    Maybe<int> m1 { nothing<int>() };
    ASSERT_TRUE(!m1.exists());
}

TEST_F(MaybeTest, CustomCtor)
{
    std::unique_ptr<int> ptr { std::make_unique<int>() };
    Maybe<std::unique_ptr<int>> m1 { just(std::move(ptr)) };
    ASSERT_TRUE(m1.exists());
}

TEST_F(MaybeTest, CustomDtor)
{
    ObjectPool<Node<int>, 5> pool;
    LinkedList<int> ll1 { &pool };
    ll1.pool() = &pool;
    {
        Maybe<LinkedList<int>> m1 { just(std::move(ll1)) };
        ASSERT_TRUE(m1.exists());
    }
    ASSERT_TRUE(ll1.size() == 0);
}

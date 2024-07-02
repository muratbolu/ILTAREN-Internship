#pragma once

#include "Node.tpp"
#include "ObjectPool.tpp"

template<typename T, unsigned N>
class LinkedList
{
public:
	constexpr LinkedList() noexcept {}
private:
	Node<T>* mHead;
	ObjectPool<Node<T>, N> mNodePool;
	const unsigned mSize{ N };
};

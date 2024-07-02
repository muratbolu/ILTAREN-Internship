#pragma once

#include "Node.tpp"
#include "ObjectPool.tpp"

template<typename T, unsigned N>
class LinkedList
{
public:
	LinkedList() noexcept {}
private:
	Node<T>* mHead;
	ObjectPool<Node, N> mNodePool;
	unsigned mSize{ N };
};

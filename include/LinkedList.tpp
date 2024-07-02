#pragma once

#include "Node.tpp"
#include "ObjectPool.tpp"

template<typename T, unsigned N>
class LinkedList
{
public:
	LinkedList() noexcept {}
private:
	Node<T>* mhead;
	unsigned msize{ N };
};

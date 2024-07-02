#pragma once

#include "Node.tpp"
#include "ObjectPool.tpp"

template<typename T, unsigned N>
class LinkedList
{
public:
	constexpr LinkedList() noexcept
	{
		mHead = construct(mSize);
	}

	constexpr Node<T>* head() const noexcept
	{
		return mHead;
	}

	constexpr Node<T>*& head() noexcept
	{
		return mHead;
	}

private:
	Node<T>* mHead;
	ObjectPool<Node<T>, N> mNodePool;
	const unsigned mSize{ N };

	constexpr Node<T>* construct(unsigned n) noexcept
	{
		if (n == 0)
		{
			return nullptr;
		}
		Node<T>* ptr = mNodePool.allocate();
		ptr->next() = construct(n - 1);
		if (ptr->next() != nullptr)
		{
			ptr->next()->prev() = ptr;
		}
		return ptr;
	}
};

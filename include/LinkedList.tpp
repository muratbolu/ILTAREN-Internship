#pragma once

#include "Node.tpp"
#include "ObjectPool.tpp"

template<typename T, unsigned N>
class LinkedList
{
public:
	constexpr LinkedList() noexcept {}
	constexpr ~LinkedList() noexcept { /* TODO */ }

	constexpr Node<T>* head() const noexcept
	{
		return mHead;
	}

	constexpr Node<T>*& head() noexcept
	{
		return mHead;
	}

	constexpr unsigned size() noexcept
	{
		return mSize;
	}

	constexpr bool push(const T& data) noexcept
	{
		Node<T>* node = mNodePool.allocate();
		if (node == nullptr)
		{
			return false;
		}
		node->data() = data;

		if (mHead == nullptr)
		{
			mHead = node;
			mSize++;
			return true;
		}
		Node<T>* curr = mHead;
		while (curr->next() != nullptr)
		{
			curr = curr->next();
		}
		curr->next() = node;
		curr->next()->prev() = curr;
		mSize++;
		return true;
	}

	constexpr T* pop() noexcept
	{
		/* TODO */
		return nullptr;
	}

private:
	Node<T>* mHead{ nullptr };
	ObjectPool<Node<T>, N> mNodePool;
	unsigned mSize{ 0 };

	/*
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
	*/
};

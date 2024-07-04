#pragma once

#include "Node.tpp"
#include "NodeIter.tpp"
#include "ObjectPool.tpp"

template<typename T>
class LinkedList
{
public:
	constexpr LinkedList(IObjectPool<Node<T>>& pool) noexcept : mNodePool{ pool } {}
	constexpr ~LinkedList() noexcept
	{
		deleteAll(mHead);
	}

	constexpr unsigned size() const noexcept
	{
		return mSize;
	}

	constexpr T* at(unsigned n) const noexcept
	{
		for (Node<T>* curr{ mHead }; curr != nullptr; curr = curr->next())
		{
			if (n == 0)
			{
				return &curr->data();
			}
			n--;
		}
		return nullptr;
	}

	constexpr T* operator[](unsigned n) const noexcept
	{
		for (Node<T>* curr{ mHead }; curr != nullptr; curr = curr->next())
		{
			if (n == 0)
			{
				return &curr->data();
			}
			n--;
		}
		return nullptr;
	}

	constexpr T* front() const noexcept
	{
		if (mHead != nullptr)
		{
			return &mHead->data();
		}
		return nullptr;
	}

	constexpr T* back() const noexcept
	{
		for (Node<T>* curr{ mHead }; curr != nullptr; curr = curr->next())
		{
			if (curr->next() == nullptr)
			{
				return &curr->data();
			}
		}
		return nullptr;
	}

	constexpr NodeIter<T> begin() noexcept
	{
		return NodeIter<T>{mHead};
	}

	constexpr NodeIter<T> end() noexcept
	{
		return NodeIter<T>{nullptr};
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
		if (mHead == nullptr)
		{
			// invalid pop
			return nullptr;
		}
		if (mHead->next() == nullptr)
		{
			// pop head
			T* result = &mHead->data();
			mNodePool.deallocate(mHead);
			mHead = nullptr;
			mSize--;
			return result;
		}

		// pop the last node
		Node<T>* curr = mHead;
		while (curr->next() != nullptr)
		{
			curr = curr->next();
		}
		curr->prev()->next() = nullptr;
		T* result = &curr->data();
		mNodePool.deallocate(curr);
		mSize--;
		return result;
	}

	constexpr void print(FILE* stream) noexcept
	{
		fputs("[", stream);
		for (Node<T>* curr{ mHead }; curr != nullptr; curr = curr->next())
		{
			fprintf(stream, "%d", curr->data());
			if (curr->next() != nullptr)
			{
				fputs(", ", stream);
			}
		}
		fputs("]", stream);
	}

private:
	Node<T>* mHead{ nullptr };
	IObjectPool<Node<T>>& mNodePool;
	unsigned mSize{ 0 };

	/* Deletes all nodes following the argument and the
	 * argument itself.
	 */
	constexpr void deleteAll(Node<T>* const node) noexcept
	{
		if (node == nullptr)
		{
			return;
		}
		Node<T>* next{ node->next() };
		mNodePool.deallocate(node);
		// tail recursive, suitable for optimization
		deleteAll(next);
	}
};

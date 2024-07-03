#pragma once

template<typename T>
class Node
{
public:
	constexpr Node() noexcept : mData{ new T{} } {}
	constexpr Node(T data) noexcept : mData{ new T{data} } {}

	constexpr ~Node() noexcept
	{
		delete mData;
	}

	constexpr T& data() const noexcept
	{
		return *mData;
	}

	constexpr Node* next() const noexcept
	{
		return mNext;
	}

	constexpr Node*& next() noexcept
	{
		return mNext;
	}

	constexpr Node* prev() const noexcept
	{
		return mPrev;
	}

	constexpr Node*& prev() noexcept
	{
		return mPrev;
	}

private:
	/* Node "owns" its data field, which is never null,
	 * and will delete it when its destructor is called.
	 */
	T* const mData;
	Node* mNext{ nullptr };
	Node* mPrev{ nullptr };
};

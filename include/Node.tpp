#pragma once

template<typename T>
class Node
{
public:
	constexpr Node() noexcept : mData{ T{} } {}
	constexpr Node(T data) noexcept : mData{ data } {}

	constexpr T data() const noexcept
	{
		return mData;
	}

	constexpr T& data() noexcept
	{
		return mData;
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
	T mData;
	Node* mNext{ nullptr };
	Node* mPrev{ nullptr };
};

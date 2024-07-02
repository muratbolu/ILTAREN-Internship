#pragma once

template<typename T>
class Node
{
public:
	constexpr Node() noexcept {}
private:
	T* mData;
	Node* mNext;
	Node* mPrev;
};

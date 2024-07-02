#pragma once

template<typename T>
class Node
{
public:
	Node() noexcept {}
private:
	T* mData;
	Node* mNext;
	Node* mPrev;
};

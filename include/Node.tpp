#pragma once

template<typename T>
class Node
{
public:
	Node() noexcept {}
private:
	T* mdata;
	Node* mnext;
	Node* mprev;
};

#pragma once

#include "Deque.tpp"

template<typename T>
class Queue : private Deque<T>
{
};

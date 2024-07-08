#pragma once

#include "Deque.tpp"

template<typename T>
class Stack : private Deque<T>
{
};

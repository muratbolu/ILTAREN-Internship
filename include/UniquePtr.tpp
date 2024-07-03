#pragma once

template<typename T> struct remove_reference { typedef T type; };
template<typename T> struct remove_reference<T&> { typedef T type; };
template<typename T> struct remove_reference<T&&> { typedef T type; };

template<typename T>
typename remove_reference<T>::type&&
move(T&& a)
{
	return a;
}

template<typename T>
class UniquePtr
{
public:
	UniquePtr() noexcept {}
	UniquePtr(const UniquePtr&) = delete;
	UniquePtr(UniquePtr&& obj) noexcept : mData{ move(obj.mData) } {}

	UniquePtr& operator=(const UniquePtr&) = delete;
	UniquePtr& operator=(UniquePtr&& obj)
	{
		mData = move(obj.mData);
		return *this;
	}

	~UniquePtr() noexcept {}
private:
	T* const mData;
};
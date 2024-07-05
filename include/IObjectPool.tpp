#pragma once

template<typename T>
class IObjectPool
{
public:
	virtual constexpr inline ~IObjectPool() noexcept {};
	virtual constexpr T* allocate() noexcept = 0;
	virtual constexpr bool deallocate(T* ptr) noexcept = 0;
};

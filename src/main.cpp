#include "static_vector.tpp"

#include <iostream>

int main()
{
	std::cout << "Hello, World!\n";
	static_vector<int, 5> vec{};
	vec[1] = 12;
	vec.back() = 15;
	std::cout << vec[1] << std::endl;
	std::cout << vec.back() << std::endl;
	vec.front() = 17;
	std::cout << std::endl;
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		std::cout << *it << std::endl;
	}
	vec.clear();
	std::cout << std::endl;
	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		std::cout << *it << std::endl;
	}
	return 0;
}

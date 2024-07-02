#pragma once

#include "static_vector.tpp"

class StaticVectorTest : public testing::Test
{
protected:
	StaticVectorTest()
	{
		vec1[0] = 3;
		vec1[1] = 5;
		vec1[2] = 7;
	}
	static_vector<int, 3> vec1;
};

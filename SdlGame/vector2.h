#pragma once
#include "type_check.h"
#include <type_traits>
template <typename  T>
class vector2
{
	
public:
	

	T x;
	T y;

	vector2(T x, T y);
	vector2();
};

template <typename T>
vector2<T>::vector2(T x, T y)
{
	static_assert(std::is_integral<T>() 
		|| std::is_floating_point<T>(), 
		"Type of vector2 must be numeral: integral or floating point");

	this->x = x;
	this->y = y;
}

template <typename T>
vector2<T>::vector2()
	:vector2<T>(0, 0)
{
}


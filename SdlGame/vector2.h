#pragma once
#include "type_check.h"
#include <type_traits>
#include <sstream>
template <typename  T>
class vector2
{
public:
	T x;
	T y;

	vector2(T x, T y);
	vector2();

	std::string to_string();
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

template <typename T>
std::string vector2<T>::to_string()
{
	std::stringstream ss;
	ss << "{ x: " << x << ", y: " << y << " } (vector2)";
	return ss.str();
}


using vector2f = vector2<float>;
using point = vector2<int>;
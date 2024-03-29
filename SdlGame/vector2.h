﻿#pragma once
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

	vector2<T> operator +(const vector2<T> & second) const;
	vector2<T> & operator +=(const vector2<T> &);
	vector2<T> operator -(const vector2<T> & second) const;
	vector2<T> & operator -=(const vector2<T> &);

	vector2<T> operator *(const float & multipl) const;
	vector2<T> operator *(const vector2<T> & multipl) const;

	bool operator==(const vector2<T>& rhs) const;

	bool operator!=(const vector2<T>& rhs) const;

	vector2<T> normalized();

	float length();
	
	void cut_negative();

	static vector2<T> lerp(vector2<T> origin, vector2<T> destination, float progress);

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
vector2<T> vector2<T>::operator+(const vector2<T>& second) const
{
	vector2<T> result;
	result.x = this->x + second.x;
	result.y = this->y + second.y;

	return result;
}

template <typename T>
vector2<T>& vector2<T>::operator+=(const vector2<T>& second)
{
	this->x += second.x;
	this->y += second.y;

	return *this;
}

template <typename T>
vector2<T> vector2<T>::operator-(const vector2<T>& second) const
{
	vector2<T> result;
	result.x = this->x - second.x;
	result.y = this->y - second.y;

	return result;
}

template <typename T>
vector2<T>& vector2<T>::operator-=(const vector2<T>& second)
{
	this->x -= second.x;
	this->y -= second.y;

	return *this;
}

template <typename T>
vector2<T> vector2<T>::operator*(const float& miltipl) const
{
	vector2<T> result;
	result.x = x*miltipl;
	result.y = y*miltipl;
	return result;
}

template <typename T>
vector2<T> vector2<T>::operator*(const vector2<T>& multipl) const
{
	return vector2<T>(this->x * multipl.x, this->y * multipl.y);
}

template <typename T>
bool vector2<T>::operator==(const vector2<T>& rhs) const
{
	return this->x == rhs.x && this->y == rhs.y;
}

template <typename T>
bool vector2<T>::operator!=(const vector2<T>& rhs) const
{
	return !(*this == rhs);
}

template <typename T>
vector2<T> vector2<T>::normalized()
{
	double sum = x + y;
	if (sum == 0)
		return vector2<T>(0, 0);
	return vector2<T>(x / sum, y / sum);
}

template <typename T>
float vector2<T>::length()
{
	return sqrt(x*x + y*y);
}

template <typename T>
void vector2<T>::cut_negative()
{
	if (x < 0) x = 0;
	if (y < 0) y = 0;
}

template <typename T>
vector2<T> vector2<T>::lerp(vector2<T> origin, vector2<T> destination, float progress)
{
	vector2f diff = destination - origin;
	return origin + diff*progress;
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

const vector2f vector2f_zero(0, 0);
const vector2f vector2f_one(1, 1);


#pragma once
#include <string>

template< class T >
struct type_check
{
	type_check();
	static const std::string value() { return "undefined"; }
	static const bool numeral() { return false; }
};

template<>
struct type_check< int >
{
	static const std::string value() { return "int"; }
	static const bool numeral() { return true; }
};

template<>
struct type_check< float >
{
	static const std::string value() { return "float"; }
	static const bool numeral() { return true; }
};

template<>
struct type_check< double >
{
	static const std::string value() { return "double"; }
	static const bool numeral() { return true; }
};

template<>
struct type_check<std::string>
{
	static const std::string value() { return "string"; }
	static const bool numeral() { return false; }
};
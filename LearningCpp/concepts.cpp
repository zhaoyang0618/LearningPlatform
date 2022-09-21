///
/// C++20的新语法:concept
/// 
#include "testlang.h"
#include <type_traits>
#include <iostream>

template<typename T>
concept IsPointer = std::is_pointer_v<T>;

template<typename T>
requires (!IsPointer<T>)
T maxValue(T a, T b)
{
	return b < a ? a : b;
}

template<typename T>
	requires IsPointer<T>
auto maxValue(T a, T b)
{
	return maxValue(*a, *b);
}

//上面的可以简化：overload resolution of concepts
//注意这里没有requires
template<typename T>
T maxValue1(T a, T b)
{
	return b < a ? a : b;
}

template<typename T>
	requires IsPointer<T>
auto maxValue1(T a, T b)
{
	return maxValue1(*a, *b);
}

//还可以这样使用
template<typename T>
T maxValue2(T a, T b)
{
	return b < a ? a : b;
}

//template<IsPointer T>
//auto maxValue2(T a, T b)
//{
//	return maxValue2(*a, *b);
//}

//甚至这样写
//you can use the concept as a type constraint when declaring parameters with auto:
auto maxValue2(IsPointer auto a, IsPointer auto b)
{
	return maxValue2(*a, *b);
}

auto maxValue3(const IsPointer auto& a, const IsPointer auto& b)
{
	return maxValue2(*a, *b); // compare values the pointers point to
}

// 这样写
// 可以采用下面更简洁的写法
//template<typename T1, typename T2>
//auto maxValue4(T1 a, T2 b) // maxValue() for values
//{
//	return b < a ? a : b; // compare values
//}

auto maxValue4(auto a, auto b) // maxValue() for values
{
	return b < a ? a : b; // compare values
}

auto maxValue4(IsPointer auto a, IsPointer auto b)
{
	return maxValue4(*a, *b);
}

//auto maxValue4(IsPointer auto a, IsPointer auto b)
//	requires std::three_way_comparable_with<decltype(*a), decltype(*b)>
//{
//	return maxValue4(*a, *b);
//}

//要求指针类型，包括智能指针，指向的对象能够比较大小
template<typename T>
concept IsPointer2 = requires(T p) {
	*p; // operator * has to be valid
	{p < p} -> std::convertible_to<bool>; // < yields bool
	p == nullptr; // can compare with nullptr
};


void testConcepts()
{
	std::cout << "test concepts" << std::endl;

	int a = 1;
	int b = 2;
	double c = 3.14159;
	std::cout << maxValue(a, b) << std::endl;
	std::cout << maxValue(&a, &b) << std::endl;

	std::cout << maxValue1(a, b) << std::endl;
	std::cout << maxValue1(&a, &b) << std::endl;

	std::cout << maxValue2(a, b) << std::endl;
	//std::cout << maxValue2(&a, &c) << std::endl;//注意这里的a和c类型不一样了, 这个无法通过编译
	std::cout << maxValue2(&a, &b) << std::endl;
	std::cout << maxValue3(&a, &b) << std::endl;

	std::cout << maxValue4(a, b) << std::endl;
	std::cout << maxValue4(&a, &c) << std::endl;//注意这里的a和c类型不一样了, 这个可以通过编译
	std::cout << maxValue4(&a, &b) << std::endl;
}

///
/// C++20�����﷨:concept
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

//����Ŀ��Լ򻯣�overload resolution of concepts
//ע������û��requires
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

//����������ʹ��
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

//��������д
//you can use the concept as a type constraint when declaring parameters with auto:
auto maxValue2(IsPointer auto a, IsPointer auto b)
{
	return maxValue2(*a, *b);
}

auto maxValue3(const IsPointer auto& a, const IsPointer auto& b)
{
	return maxValue2(*a, *b); // compare values the pointers point to
}

// ����д
// ���Բ������������д��
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

//Ҫ��ָ�����ͣ���������ָ�룬ָ��Ķ����ܹ��Ƚϴ�С
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
	//std::cout << maxValue2(&a, &c) << std::endl;//ע�������a��c���Ͳ�һ����, ����޷�ͨ������
	std::cout << maxValue2(&a, &b) << std::endl;
	std::cout << maxValue3(&a, &b) << std::endl;

	std::cout << maxValue4(a, b) << std::endl;
	std::cout << maxValue4(&a, &c) << std::endl;//ע�������a��c���Ͳ�һ����, �������ͨ������
	std::cout << maxValue4(&a, &b) << std::endl;
}

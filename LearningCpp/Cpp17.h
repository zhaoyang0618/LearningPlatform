///
/// �ο���Դ��
/// C++17 In Detail
/// 
#pragma once
#include <string>
#include <vector>
#include <iostream>

/// <summary>
/// �﷨�����һЩ����
/// </summary>
void testMap();
void testPrinting();
void testSample();

/// <summary>
/// �淶��һЩ���ʽ�����˳��
/// </summary>
void testStricterExpressionEvaluationOrder();

/// <summary>
/// ����ʡ��:�����������
/// </summary>
/// <returns></returns>
int testGuaranteedCopyElision();

/// <summary>
/// �ڴ����
/// </summary>
void testDynamicMemoryAllocationforOverAlignedData();

/// �쳣��Ϊ����ϵͳ��һ����
/// You can now have two function overloads: one with noexcept and one without it.
/// ���Զ��庯�����أ�һ����noexcept��һ��û��
/// void (*p)();
/// void (**pp)() noexcept = &p; // error: cannot convert to
///							// pointer to noexcept function
/// struct S { typedef void (*p)(); operator p(); };
/// void (*q)() noexcept = S(); // error: cannot convert to
///							// pointer to noexcept

///�������https://blog.csdn.net/fireflylane/article/details/84076588
template<typename T> void linePrinter(const T& x)
{
	if constexpr (std::is_integral_v<T>)
	{
		std::cout << "num: " << x << std::endl;
	}
	else if constexpr (std::is_floating_point_v<T>)
	{
		const auto frac = x - static_cast<long>(x);
		std::cout << "flt: " << x << ", frac " << frac << std::endl;
	}
	else if constexpr (std::is_pointer_v<T>)
	{
		std::cout << "ptr, ";
		linePrinter(*x);
	}
	else
	{
		std::cout << x << std::endl;
	}
}

template<typename... Args> void printWithInfo(Args... args)
{
	//Fold Expressions
	(linePrinter(args), ...);
}

/// �ṹ����
/// Structured Binding Declarations
/// Such syntax is called structured binding expression.
/// �����﷨��ʽ:
/// auto [a, b, c, ...] = expression;
/// auto [a, b, c, ...]{ expression };
/// auto [a, b, c, ...](expression);
/// �����ڱ�����������������µĲ�����
/// auto tempTuple = expression;
/// using a = tempTuple.first;
/// using b = tempTuple.second;
/// using c = tempTuple.third;
/// Conceptually, the expression is copied into a tuple-like object (tempTuple) with member variables
/// that are exposed through a, band c.However, the variables a, band c are not references, they are
/// aliases(or bindings) to the hidden object member variables.The temporary object has a unique name
/// assigned by the compiler.
/// һЩ����
/// const modifiers��������:
///		const auto [a, b, c, ...] = expression;
/// References (����):
///		auto& [a, b, c, ...] = expression;
///		auto&& [a, b, c, ...] = expression;
/// You can also add [[attribute]]:
///		[[maybe_unused]] auto& [a, b, c, ...] = expression;
/// Structured Binding Declaration cannot be declared constexpr
/// constexpr auto [x, y] = std::pair(0, 0);//����޷�ͨ������: structured binding declaration cannot be 'constexpr'
/// �п�����C++20������
/// ������нṹ���󶨵ģ�
/// 
void testStructuredBindingExpression();

///
/// Init Statement for if and switch
/// �﷨: if (init; condition) and switch (init; condition).
/// In the init section you can specify a new variable and then check it in the condition section. 
/// The variable is visible only in if / else scope.
void testInitStatementforifandswitch();

///
/// Inline Variables
/// With Non-Static Data Member Initialisation introduced in C++11, you can now declare and initialise member variables in one place :
/// Still, with static variables (or const static) you usually need to define it in some cpp file.
/// C++11 and constexpr keyword allow you to declare and define static variables in one place, but
/// it��s limited to constant expressions only.
/// Previously, only methods / functions could be specified as inline, but now you can do the same with
/// variables, inside a header file.
class User
{
	//�����ĵط����г�ʼ��: 
	int _age{ 0 };
	std::string _name{ "unknown" };
};

// inside a header file:
struct MyClass
{
	static const int sValue;
};

// later in the same header file:
inline int const MyClass::sValue = 777;

//Also, note that constexpr variables are inline implicitly, so there��s no need to use constexpr inline myVar = 10; .
//An inline variable is also more flexible than a constexpr variable as it doesn��t have to be initialised
//with a constant expression.For example, you can initialise an inline variable with rand(), but it��s
//not possible to do the same with constexpr variable.

//����ֱ������������
struct MyClass2
{
	inline static const int sValue = 777;
};

//������Ĵ���MyClass3���Ա��滻ΪMyClass4
class MyClass3
{
	static inline int Seed(); // static method
};

inline int MyClass3::Seed() {
	static const int seed = rand();
	return seed;
}

class MyClass4
{
	static inline int seed = rand();
};

///
/// constexpr Lambda Expressions
/// C++11����lambda���ʽ�ͳ������ʽ(const expression, ͨ��constexpr)
/// C++17�����߿��Խ����
/// ��׼�� 10.1.5 The constexpr specifier [dcl.constexpr]
/// The definition of a constexpr function shall satisfy the following requirements:
/// * it shall not be virtual (13.3);
/// * its return type shall be a literal type; A > -each of its parameter types shall be a literal type;
///		A > -its function - body shall be = delete, = default, or a compound - statement that does not
///		contain:
///			�C an asm - definition,
///			�C a goto statement,
///			�C an identifier label(9.1),
///			�C a try - block, or
///			�C a definition of a variable of non - literal type or of static or thread storage duration or for
///				which no initialisation is performed.
void testConstexprLambda();

///
/// Nested Namespaces
/// �����ø����յķ�ʽ��д�����ռ䣬��C#, Java���ӽ�
//namespace MySuperCompany {
//	namespace SecretProject {
//		namespace SafetySystem {
//			class SuperArmor {
//				// ...
//			};
//			class SuperShield {
//				// ...
//			};
//		} // SafetySystem
//	} // SecretProject
//} // MySuperCompany
//��һ�ָ����շ�ʽ���£�
//namespace MySuperCompany::SecretProject::SafetySystem {
//	class SuperArmor {
//		// ...
//	};
//	class SuperShield {
//		// ...
//	};
//}

///
/// ģ��
/// 
void testTemplate();

template<typename T>
struct MyType
{
	T str;
};

//�Զ���Deduction guide
// custom deduction guide
MyType(const char*)->MyType<std::string>;

//Another example of custom deduction guide is overload :
template<class... Ts>
struct overload : Ts... { using Ts::operator()...; };
template<class... Ts>
overload(Ts...)->overload<Ts...>; // deduction guide

//Fold Expressions
//C++11 �����˿ɱ�ģ�壺variadic templates
//Still, variadic templates required some additional code when you wanted to implement ��recursive��
//functions like sum, all.You had to specify rules for the recursion.
template<typename T1, typename... T>
auto SumCpp11(T1 s, T... ts) {
	return s + SumCpp11(ts...);
}
//C++17�п��Լ�
template<typename ...Args> auto sum(Args ...args)
{
	return (args + ... + 0);
}
// or even:
template<typename ...Args> auto sum2(Args ...args)
{
	return (args + ...);
}

template<typename ...Args>
void FoldPrint(Args&&... args)
{
	(std::cout << ... << std::forward<Args>(args)) << '\n';
}

template<typename ...Args>
void FoldSeparateLine(Args&&... args)
{
	auto separateLine = [](const auto& v) {
		std::cout << v << '\n';
	};
	(..., separateLine(std::forward<Args>(args))); // over comma operator
}

//The technique with fold over the comma operator is handy. Another example of it might be a special version of push_back :
//������������
template<typename T, typename... Args>
void push_back_vec(std::vector<T>& v, Args&&... args)
{
	(v.push_back(std::forward<Args>(args)), ...);
}


//if constexpr



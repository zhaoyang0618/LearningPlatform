#include "CppGrammer.h"
#include <tuple>
#include <iostream>
#include <functional>

//https://zhuanlan.zhihu.com/p/428668958
void testBind();

std::tuple<int, double, char> getTuple()
{
	return std::tuple<int, double, char>(5, 1.2, 'b');
}

//使用auto和make_系列函数
auto getTuple2()
{
	return std::make_tuple(5, 1.2, 'b');
}

//lambda表达式
void call1(int arg, std::function<void(int)> func)
{
	func(arg);
}

void call2(std::function<void()> func)
{
	func();
}

//class相关
//构造函数
//如果没有定义任何构造函数，编译器会产生一个默认构造函数（无参数构造函数）,其中成员变量不会被初始化
//其他特殊成员函数
//析构函数
//移动构造函数；移动赋值函数；复制构造函数；复制赋值函数
//delete: 禁止
//default:默认方式实现
//成员初始化
class MyRectangle
{
public:
	//这个用法在C++11之前也是非法的
	int x = 10;
	int y = 5;
	MyRectangle(int x, int y): x(x), y(y) {}
	//注意这个用法,C++11之前是非法的
	MyRectangle() : MyRectangle(10, 5) {}
};

class A
{
public:
	//explicitly include default constructor
	A() = default;
	//Explicitly include default destructor
	~A() = default;
	//Disable move constructor
	A(A&&) noexcept = delete;
	//Disable move assignment operator
	A& operator=(A&&) noexcept = delete;
	//Disable copy constructor
	A(const A&) noexcept = delete;
	//Disable copy assignment operator
	A& operator=(const A&) noexcept = delete;
};

class Foo1
{
public:
	int x = 0;
	int& ref1 = x;
	int& ref2;
	Foo1();
};

Foo1::Foo1() : ref2(x) 
{
}

//各种初始化方式
class MyClass
{
public:
	int i;
	MyClass() = default;
	MyClass(int x) :i(x) {}
};

class NewClass
{
public:
	NewClass(std::initializer_list<int>);
};

NewClass::NewClass(std::initializer_list<int> args)
{
	for (auto x : args)
		std::cout << x << " ";
	std::cout << std::endl;
}

class TestClass
{
public:
	int a = 1;
	int b = 2;
};

//继承之间，如何借用构造函数
class B1
{
public:
	int x;
	B1(int a) :x(a) {}
};

class D1 : public B1
{
public:
	D1(int i) :B1(i) {}
};

//或者如下语法
class D2 : public B1
{
public:
	using B1::B1;//继承所有的B1构造函数
	int y{ 0 };	//自己的成员初始化
};

//虚函数重载：overide; final
//final还可以用于class
class B3
{
	virtual void foo() {}
	virtual void foo1() final {}
};

class D3 :public B3
{
	void foo() override { }
	//void foo1() {}//这是不允许的
};
class B4 final {};	//此时B4不允许有子类
//class D4 : B4 {};//也是不允许的

void CppGrammerDemo()
{
	//访问tuple
	auto mytuple = getTuple();
	std::cout << std::get<0>(mytuple)
		<< std::get<char>(mytuple);

	//tie访问tuple
	int i; 
	double d;
	std::tie(i, d, std::ignore) = getTuple2();
	std::cout << i << d;

	//结构化绑定C++17引入
	auto [i2, d2, c] = getTuple2();
	std::cout << i2;

	//lambda表达式
	auto sum1 = [](int x, int y)->int {
		return x + y;
	};
	auto sum2 = [](int x, int y) {
		return x + y;
	};
	//C++14
	auto sum3 = [](auto x, auto y) {
		return x + y;
	};

	auto a1 = sum1(1, 2);
	auto a2 = sum2(1, 2);
	auto a3 = sum3(1, 2);

	//
	auto printSquare1 = [](int x) {
		std::cout << x * x << std::endl;
	};

	call1(2, printSquare1);

	int j = 2;
	//此时j已经传入进去了,by value
	auto printSquare2 = [j]() {
		std::cout << j * j << std::endl;
	};
	auto printSquare3 = [&j]() {
		std::cout << j * j << std::endl;
	};
	call2(printSquare2);
	call2(printSquare3);

	j = 3;
	call2(printSquare2);
	call2(printSquare3);
	j = 4;
	call2(printSquare2);
	call2(printSquare3);

	//C++20
	//a lambda that does not capture any variabkes is called stateless.
	//C++20 added the ability to make stateless lambdas default constructible and assignable
	//auto x = [] {return 3; };
	//decltype(x) y;

	//类:class
	//直接初始化:Direct Initialization
	MyClass m1(5);
	MyClass m2;//注意这个时候m2.i的值是不定义的，也就是不知道是啥,

	//Value Initialization
	//最好不要这么用
	//不过这个时候m3和m4中的i似乎是有默认值的(0)
	const MyClass& m3 = MyClass();
	MyClass&& m4 = MyClass();	//alternative

	//Copy Initialization
	MyClass m5 = MyClass();//copy temporary object to a
	MyClass m6 = m5;	//copy object a to b

	//New Initialization
	//使用new创建的对象必须自行负责delete
	MyClass* m7 = new MyClass();
	MyClass& m8 = *new MyClass();//这种用法很少见
	delete m7;
	delete& m8;

	//Aggregate Initialization
	//class type does not include any constructors, virtual functions, or base classes
	//?MyClass不是有构造函数吗？
	MyClass m9 = { 2 };	//这个似乎是C语言初始化结构体(struct),数组的方式

	//Uniform Initialization
	//C++11引入的
	MyClass m10{ 3 };
	//call parameterless constructor
	MyClass m11{  };
	//call copy constructor
	MyClass m12{ m11 };
	//
	NewClass n{ 1,2,3 };

	//C++20引入新的初始化方式
	//TestClass o1{ .a = 3, .b = 4};
	//TestClass o2{ .a = 5 };
	//TestClass o3{ .b = 6 };
	//对顺序有要求,下面两种都是错误的
	//TestClass o4{ .b = 0, .a = 1 };//out of order
	//TestClass o5{ .a = 5, 3 };//不能混合使用

	D2 dd(3);
}

void MyFunc1() { std::cout << "\n\nDelay execute function.\n\n"; }
void MyFunc2(int a, int b) { std::cout << std::endl << a + b << std::endl; }
void MyFunc3(int a, int b, int c, int d) { std::cout << std::endl << a + b + c + d << std::endl; }

void testBind()
{
	// 绑定一个无参函数，建立了一个执行对象 
	auto delayFunc1 = std::bind(MyFunc1);
	// 在任何需要的调用执行 
	delayFunc1();

	// 使用参数占位符，相当于定义了执行对象调用时参数个数 
	auto delayFunc2 = std::bind(MyFunc2, std::placeholders::_1, std::placeholders::_2);
	// 调用时必须传入两个参数 
	delayFunc2(11, 22);

	// 在声明时可显示传入参数，并且将需要在调用时传入的参数放置参数占位符，显示传入的参数和参数占位符可以交替书写 
	auto delayFunc3 = std::bind(MyFunc3, 11, std::placeholders::_1, 22, std::placeholders::_2);
	delayFunc3(33, 0); // 分别作为参数占位符 std::placeholders::_1 和 std::placeholders::_2     

}



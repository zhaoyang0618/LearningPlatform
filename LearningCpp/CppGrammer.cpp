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

//ʹ��auto��make_ϵ�к���
auto getTuple2()
{
	return std::make_tuple(5, 1.2, 'b');
}

//lambda���ʽ
void call1(int arg, std::function<void(int)> func)
{
	func(arg);
}

void call2(std::function<void()> func)
{
	func();
}

//class���
//���캯��
//���û�ж����κι��캯���������������һ��Ĭ�Ϲ��캯�����޲������캯����,���г�Ա�������ᱻ��ʼ��
//���������Ա����
//��������
//�ƶ����캯�����ƶ���ֵ���������ƹ��캯�������Ƹ�ֵ����
//delete: ��ֹ
//default:Ĭ�Ϸ�ʽʵ��
//��Ա��ʼ��
class MyRectangle
{
public:
	//����÷���C++11֮ǰҲ�ǷǷ���
	int x = 10;
	int y = 5;
	MyRectangle(int x, int y): x(x), y(y) {}
	//ע������÷�,C++11֮ǰ�ǷǷ���
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

//���ֳ�ʼ����ʽ
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

//�̳�֮�䣬��ν��ù��캯��
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

//���������﷨
class D2 : public B1
{
public:
	using B1::B1;//�̳����е�B1���캯��
	int y{ 0 };	//�Լ��ĳ�Ա��ʼ��
};

//�麯�����أ�overide; final
//final����������class
class B3
{
	virtual void foo() {}
	virtual void foo1() final {}
};

class D3 :public B3
{
	void foo() override { }
	//void foo1() {}//���ǲ������
};
class B4 final {};	//��ʱB4������������
//class D4 : B4 {};//Ҳ�ǲ������

void CppGrammerDemo()
{
	//����tuple
	auto mytuple = getTuple();
	std::cout << std::get<0>(mytuple)
		<< std::get<char>(mytuple);

	//tie����tuple
	int i; 
	double d;
	std::tie(i, d, std::ignore) = getTuple2();
	std::cout << i << d;

	//�ṹ����C++17����
	auto [i2, d2, c] = getTuple2();
	std::cout << i2;

	//lambda���ʽ
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
	//��ʱj�Ѿ������ȥ��,by value
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

	//��:class
	//ֱ�ӳ�ʼ��:Direct Initialization
	MyClass m1(5);
	MyClass m2;//ע�����ʱ��m2.i��ֵ�ǲ�����ģ�Ҳ���ǲ�֪����ɶ,

	//Value Initialization
	//��ò�Ҫ��ô��
	//�������ʱ��m3��m4�е�i�ƺ�����Ĭ��ֵ��(0)
	const MyClass& m3 = MyClass();
	MyClass&& m4 = MyClass();	//alternative

	//Copy Initialization
	MyClass m5 = MyClass();//copy temporary object to a
	MyClass m6 = m5;	//copy object a to b

	//New Initialization
	//ʹ��new�����Ķ���������и���delete
	MyClass* m7 = new MyClass();
	MyClass& m8 = *new MyClass();//�����÷����ټ�
	delete m7;
	delete& m8;

	//Aggregate Initialization
	//class type does not include any constructors, virtual functions, or base classes
	//?MyClass�����й��캯����
	MyClass m9 = { 2 };	//����ƺ���C���Գ�ʼ���ṹ��(struct),����ķ�ʽ

	//Uniform Initialization
	//C++11�����
	MyClass m10{ 3 };
	//call parameterless constructor
	MyClass m11{  };
	//call copy constructor
	MyClass m12{ m11 };
	//
	NewClass n{ 1,2,3 };

	//C++20�����µĳ�ʼ����ʽ
	//TestClass o1{ .a = 3, .b = 4};
	//TestClass o2{ .a = 5 };
	//TestClass o3{ .b = 6 };
	//��˳����Ҫ��,�������ֶ��Ǵ����
	//TestClass o4{ .b = 0, .a = 1 };//out of order
	//TestClass o5{ .a = 5, 3 };//���ܻ��ʹ��

	D2 dd(3);
}

void MyFunc1() { std::cout << "\n\nDelay execute function.\n\n"; }
void MyFunc2(int a, int b) { std::cout << std::endl << a + b << std::endl; }
void MyFunc3(int a, int b, int c, int d) { std::cout << std::endl << a + b + c + d << std::endl; }

void testBind()
{
	// ��һ���޲κ�����������һ��ִ�ж��� 
	auto delayFunc1 = std::bind(MyFunc1);
	// ���κ���Ҫ�ĵ���ִ�� 
	delayFunc1();

	// ʹ�ò���ռλ�����൱�ڶ�����ִ�ж������ʱ�������� 
	auto delayFunc2 = std::bind(MyFunc2, std::placeholders::_1, std::placeholders::_2);
	// ����ʱ���봫���������� 
	delayFunc2(11, 22);

	// ������ʱ����ʾ������������ҽ���Ҫ�ڵ���ʱ����Ĳ������ò���ռλ������ʾ����Ĳ����Ͳ���ռλ�����Խ�����д 
	auto delayFunc3 = std::bind(MyFunc3, 11, std::placeholders::_1, 22, std::placeholders::_2);
	delayFunc3(33, 0); // �ֱ���Ϊ����ռλ�� std::placeholders::_1 �� std::placeholders::_2     

}



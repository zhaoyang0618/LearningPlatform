///
/// ������һЩ�����ϼ����ļ��ɣ��Ƚ���ɢ�Ĵ���
/// 
#include "LearningCpp.h"
#include <iostream>

class A
{
public:
	~A()
	{
		std::cout << "A" << std::endl;
	}
};

class B : public A
{
public:
	~B()
	{
		std::cout << "B" << std::endl;
	}
};

class manager
{
public:
	virtual ~manager() { std::cout << "manager" << std::endl; }
};

template<class T>
class manager_ptr : public manager
{
public:
	manager_ptr(T p) : ptr(p) {}
	~manager_ptr() { delete ptr; std::cout << "manager_ptr" << std::endl; }

private:
	T ptr;

};


template<class T>
class _shatr_ptr
{
public:
	template<class Y>
	_shatr_ptr(Y* p) 
	{
		ptr_manager = new manager_ptr<Y*>(p);
	}

	~_shatr_ptr()
	{
		delete ptr_manager;
	}

private:
	manager* ptr_manager;
};

template<typename T>
class MySharedV2
{
public:
	template<typename T2>
	MySharedV2(T2* p)
	{
		data_ = p;
		deleter_ = [p]() {delete p; };
	}

	~MySharedV2()
	{
		deleter_();
	}

	T* operator->()
	{
		return data_;
	}

private:
	std::function<void()> deleter_;
	T* data_;
};

/// <summary>
/// 
/// </summary>
extern void testPtrDelete()
{
	std::cout << "�������飬���ɾ����������" << std::endl;
	_shatr_ptr<A> ptr(new B);
}

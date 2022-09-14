#include "Cpp20.h"

#include <vector>
#include <list>
#include <deque>
#include <map>
#include <ranges>
#include <algorithm>
#include <format>
#include <iostream>
#include <string_view>
#include <iomanip>
#include <iterator>
#include <coroutine>

namespace stdr = std::ranges;
namespace stdrv = std::ranges::views;

// https://en.cppreference.com/w/cpp/ranges/lazy_split_view
// P2210R2: a temporary patch until online g++ >= 12
//#define lazy_split_view split_view
//#define lazy_split split

template<template<typename> typename Container, typename Arg = std::string>
concept IsSplitContainer =
std::same_as<Container<Arg>, std::vector<Arg>> ||
std::same_as<Container<Arg>, std::deque <Arg>> ||
std::same_as<Container<Arg>, std::list  <Arg>>;

template<template<typename> typename Container = std::vector>
	requires IsSplitContainer<Container>
Container<std::string> Split(std::string_view string, std::string_view delimiter)
{
	Container<std::string> myCont;
	std::string tmp;
	stdr::for_each(string | stdrv::lazy_split(delimiter), [&](auto&& views)
		{
			stdr::for_each(views, [&tmp](auto&& elem) { tmp.push_back(elem); });
			myCont.emplace_back(tmp);
			tmp.clear();
		});
	return myCont;
}

auto print = [](auto const& view)
{
	// `view` is a std::views::lazy_split_view::/*outer_iterator*/::value_type

	for (std::cout << "{ "; const auto element : view)
		std::cout << element << ' ';
	std::cout << "} ";
};

/// <summary>
/// 管道符|的重载
/// https://zhuanlan.zhihu.com/p/436956716
/// </summary>

//namespace std
//{
//	namespace ranges
//	{
//		namespace views
//		{
//			namespace __adaptor
//			{
//				struct _RangeAdaptorClosure {
//					// range | adaptor is equivalent to adaptor(range).
//					template <typename _Self, typename _Range>
//						requires std::derived_from<remove_cvref_t<_Self>, _RangeAdaptorClosure>&& __adaptor_invocable<_Self, _Range>
//						friend constexpr auto operator|(_Range&& __r, _Self&& __self) {
//						return std::forward<_Self>(__self)(std::forward<_Range>(__r));
//					}
//
//					// Compose the adaptors __lhs and __rhs into a pipeline, returning
//					// another range adaptor closure object.
//					template <typename _Lhs, typename _Rhs>
//						requires derived_from<_Lhs, _RangeAdaptorClosure>&&
//					derived_from<_Rhs, _RangeAdaptorClosure>
//						friend constexpr auto operator|(_Lhs __lhs, _Rhs __rhs) {
//						return _Pipe<_Lhs, _Rhs>{std::move(__lhs), std::move(__rhs)};
//					}
//				};
//			}
//		}
//	}
//}

//class Add : public std::ranges::views::__adaptor::_RangeAdaptor<Add> {
//public:
//	constexpr auto operator()(int range, int arg) const { return range + arg; }
//
//	using _RangeAdaptor<Add>::operator();
//	// _S_arity表示函数参数个数
//	static constexpr int _S_arity = 2;
//};
//
//class Sub : public std::ranges::views::__adaptor::_RangeAdaptor<Sub> {
//public:
//	constexpr auto operator()(int range, int arg) const { return range - arg; }
//
//	using _RangeAdaptor<Sub>::operator();
//	static constexpr int _S_arity = 2;
//};

/// <summary>
/// Range和View示例
/// </summary>
void ComposeRangeAndViews()
{
	namespace vws = std::views;
	std::vector<int> vals { 0, 1,2,3,4,};
	//
	std::map<std::string, int> composers{
		{"Bach", 1685},
		{"Mozart", 1756},
		{"Beethoven", 1770},
		{"Tchaikovsky", 1840},
		{"Chopin", 1810},
		{"Vivaldi", 1678},
	};
	//
	// iterate over the names of the first 3 composers born since 1700:
	for (const auto& elem : composers 
		| vws::filter([](const auto& y) { // since 1700
			return y.second >= 1700;
			})
		| vws::take(3) // first 3
		| vws::keys // keys/names only
				) 
	{
		std::cout << "- " << elem << '\n';
	}

}

/// <summary>
/// C++20 now can map operator != to operator == with arbitrary order of the operands.
/// </summary>
struct NullTerm
{
	bool operator ==(auto pos) const
	{
		return *pos == '\0';
	}
};

/// <summary>
/// 哨兵
/// </summary>
void testSentinel1()
{
	const char* rawString = "hello world";
	// iterate over the range of the begin of rawString and its end:
	for (auto pos = rawString; pos != NullTerm{}; ++pos) {
		std::cout << ' ' << *pos;
	}
	std::cout << '\n';
	// call range algorithm with iterator and sentinel:
	std::ranges::for_each(rawString, // begin of range
		NullTerm{}, // end is null terminator
		[](char c) {
			std::cout << ' ' << c;
		});
	std::cout << '\n';
}

void testSentinel2()
{
	const char* rawString = "hello world";
	// define a range of a raw string and a null terminator:
	std::ranges::subrange rawStringRange{ rawString, NullTerm{} };

	// use the range in an algorithm:
	std::ranges::for_each(rawStringRange,
		[](char c) {
			std::cout << ' ' << c;
		});
	std::cout << '\n';
	// range-based for loop also supports iterator/sentinel:
	for (char c : rawStringRange) {
		std::cout << ' ' << c;
	}
	std::cout << '\n';
}

template<auto END>
struct EndValue 
{
	bool operator== (auto pos)const
	{
		return *pos == END;
	}
};

void testSentinel3()
{
	std::vector coll = { 42, 8, 0, 15, 7, -1 }; // -1 marks the end
	// define a range the begin of coll and the element 7 as end:
	std::ranges::subrange range{ coll.begin(), EndValue<7>{} };
	// sort the elements of this range:
	std::ranges::sort(range);
	// print the elements of the range:
	std::ranges::for_each(range,
		[](auto val) {
			std::cout << ' ' << val;
		});
	std::cout << '\n';
	// print all elements of coll up to -1:
	std::ranges::for_each(coll.begin(), EndValue<-1>{},
		[](auto val) {
			std::cout << ' ' << val;
		});
	std::cout << '\n';
}

void testRangeAndCount()
{
	std::vector<int> coll{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	auto pos5 = std::ranges::find(coll, 5);
	//counted中数量必须保证足够，所以这里有一个检测
	//如果数量不足，程序会有未定义行为，不可取
	if (std::ranges::distance(pos5, coll.end()) >= 3) {
		for (int val : std::views::counted(pos5, 3)) {
			std::cout << val << ' ';
		}
	}
	std::cout << '\n';
}

template<std::ranges::input_range Range>
std::ranges::range_value_t<Range> maxValue(Range&& rg)
{
	if (std::ranges::empty(rg)) {
		return std::ranges::range_value_t<Range>{};
	}
	auto pos = std::ranges::begin(rg);
	auto max = *pos;
	while (++pos != std::ranges::end(rg)) {
		if (*pos > max) {
			max = *pos;
		}
	}
	return max;
}

/// <summary>
/// 延迟计算
/// </summary>
void testFiltTrans()
{
	namespace vws = std::views;
	std::vector<int> coll{ 8, 15, 7, 0, 9 };
	// define a view:
	auto vColl = coll
		| vws::filter([](int i) {
		std::cout << " filter " << i << '\n';
		return i % 3 == 0;
			})
		| vws::transform([](int i) {
				std::cout << " trans " << i << '\n';
				return -i;
			});

	// and use it:
	std::cout << "*** coll | filter | transform:\n";
	for (int i : vColl) {
		std::cout << " => " << i << '\n';
	}

}

void testTransFilt()
{
	namespace vws = std::views;
	std::vector<int> coll{ 8, 15, 7, 0, 9 };

	// define a view:
	auto vColl = coll
		| vws::transform([](int i) {
		std::cout << " trans: " << i << '\n';
		return -i;
			})
		| vws::filter([](int i) {
				std::cout << "  filt: " << i << '\n';
				return i % 3 == 0;
			});

	// and use it:
	std::cout << "*** coll | transform | filter:\n";
	for (int i : vColl) {
		std::cout << "   => " << i << '\n';
	}
}

void testCpp20()
{
	//std::cout << "C++ 20 学习" << std::endl;
	std::string str = "Hello233C++20233and233New233Spilt";
	std::string delimiter = "233";
	std::list<std::string>&& strCont = Split<std::list>(str, delimiter);
	//auto&& strCont = Split<std::list>(str, delimiter);
	std::cout << "分解之后: " << std::endl;
	stdr::for_each(strCont, [](auto&& x) { std::cout << std::format("{} ", x); });
	std::cout << "" << std::endl;

	//
	//constexpr static auto source = { 0, 1,0, 2,3,0, 4,5,6,0, 7,8,9 };
	//constexpr int delimiter{ 0 };
	//constexpr std::ranges::lazy_split_view outer_view{ source, delimiter };
	//std::cout << "splits[" << std::ranges::distance(outer_view) << "]:  ";
	//for (auto const& inner_view : outer_view)
	//	print(inner_view);

	//constexpr std::string_view hello{ "Hello C++ 20 !" };
	//std::cout << "\n" "substrings: ";
	//std::ranges::for_each(hello | std::views::lazy_split(' '), print);

	//constexpr std::string_view text{ "Hello-+-C++-+-20-+-!" };
	//constexpr std::string_view delim{ "-+-" };
	//std::cout << "\n" "substrings: ";
	//std::ranges::for_each(text | std::views::lazy_split(delim), print);
	//std::cout << std::endl;

	//
	std::cout << "管道符: " << std::endl;
	using namespace std::views;
	std::vector<int> v{ 1,2,3,4,5,6,7,8 };
	auto res = all(v) | filter([](int a) { return a % 2; }) | take(3);
	for (const auto& i : res) {
		std::cout << i << std::endl;
	}

	//
	ComposeRangeAndViews();
	//
	testSentinel1();
	testSentinel2();
	testSentinel3();
	testRangeAndCount();
	testFiltTrans();
	testTransFilt();
}
//output: Hello C++20 and New Spilt


//
template<std::size_t N>
struct array
{
	char array[N];
	enum { size = N };
};
template<int I>
struct value_of
{};
template<>
struct value_of<1>
{
	enum { value = true };
};
template<>
struct value_of<2>
{
	enum { value = false };
};
void* erase(...);
struct is_cpp20
{
	static array<1> deduce_type(std::vector<int>::size_type);
	static array<2> deduce_type(...);
	static std::vector<int> v;
	static int i;
	enum { value = value_of<sizeof(deduce_type(erase(v, i)))>::value };
};
struct is_cpp17
{
	static array<1> deduce_type(char*);
	static array<2> deduce_type(const char*);
	static std::string s;
	enum { value = value_of<sizeof(deduce_type(s.data()))>::value };
};

int cbegin(...);

struct is_cpp14
{
	static array<1> deduce_type(std::string::const_iterator);
	static array<2> deduce_type(int);
	enum { value = value_of<sizeof(deduce_type(cbegin(std::string())))>::value };
};
int move(...);

struct is_cpp11
{
	template<class T>
	static array<1> deduce_type(T);
	static array<2> deduce_type(int);
	static std::string s;
	enum { value = value_of<sizeof(deduce_type(move(s)))>::value };
};
enum {
	cpp_year =
	is_cpp20::value ? 2020 :
	is_cpp17::value ? 2017 :
	is_cpp14::value ? 2014 :
	is_cpp11::value ? 2011 :
	2003
};

void ExploringCpp20()
{
	//
	std::cout << "C++ " << std::setfill('0') << std::setw(2) << cpp_year % 100 << '\n';
	std::cout << "C++ " << std::setw(2) << (__cplusplus / 100) % 100 << '\n';
}

//https://zhuanlan.zhihu.com/p/561623494
////!coro_ret 协程函数的返回值，内部定义promise_type，承诺对象
//template <typename T>
//struct coro_ret
//{
//	struct promise_type;
//	using handle_type = std::coroutine_handle<promise_type>;
//	//! 协程句柄
//	handle_type coro_handle_;
//
//	coro_ret(handle_type h) 
//		: coro_handle_(h)
//	{
//	}
//
//	coro_ret(const coro_ret&) = delete;
//	coro_ret(coro_ret&& s)
//		: coro_handle_(s.coro_)
//	{
//		s.coro_handle_ = nullptr;
//	}
//	~coro_ret()
//	{
//		//!自行销毁
//		if (coro_handle_)
//			coro_handle_.destroy();
//	}
//	coro_ret& operator=(const coro_ret&) = delete;
//	coro_ret& operator=(coro_ret&& s)
//	{
//		coro_handle_ = s.coro_handle_;
//		s.coro_handle_ = nullptr;
//		return *this;
//	}
//	//!恢复协程，返回是否结束
//	bool move_next()
//	{
//		coro_handle_.resume();
//		return coro_handle_.done();
//	}
//
//	//!通过promise获取数据，返回值
//	T get()
//	{
//		return coro_handle_.promise().return_data_;
//	}
//
//	//!promise_type就是承诺对象，承诺对象用于协程内外交流
//	struct promise_type
//	{
//		promise_type() = default;
//		~promise_type() = default;
//		​
//		//!生成协程返回值
//		auto get_return_object()
//		{
//			return coro_ret<T>{handle_type::from_promise(*this)};
//		}
//		​
//		//! 注意这个函数,返回的就是awaiter
//		//! 如果返回std::suspend_never{}，就不挂起，
//		//! 返回std::suspend_always{} 挂起
//		//! 当然你也可以返回其他awaiter
//		auto initial_suspend()
//		{
//			//return std::suspend_never{};
//			return std::suspend_always{};
//		}
//		//!co_return 后这个函数会被调用
//		void return_value(T v)
//		{
//			return_data_ = v;
//			return;
//		}
//
//		//!
//		auto yield_value(T v)
//		{
//			std::cout << "yield_value invoked." << std::endl;
//			return_data_ = v;
//			return std::suspend_always{};
//		}
//		//! 在协程最后退出后调用的接口。
//		//! 若 final_suspend 返回 std::suspend_always 则需要用户自行调用
//		//! handle.destroy() 进行销毁，但注意final_suspend被调用时协程已经结束
//		//! 返回std::suspend_always并不会挂起协程（实测 VSC++ 2022）
//		auto final_suspend() noexcept
//		{
//			std::cout << "final_suspend invoked." << std::endl;
//			return std::suspend_always{};
//		}
//		//
//		void unhandled_exception()
//		{
//			std::exit(1);
//		}
//		//返回值
//		T return_data_;
//	};
//
//};
//​
//​
////这就是一个协程函数
//coro_ret<int> coroutine_7in7out()
//{
//	//进入协程看initial_suspend，返回std::suspend_always{};会有一次挂起
//	std::cout << "Coroutine co_await std::suspend_never" << std::endl;
//
//	//co_await std::suspend_never{} 不会挂起
//	co_await std::suspend_never{};
//	std::cout << "Coroutine co_await std::suspend_always" << std::endl;
//	co_await std::suspend_always{};
//	​
//		std::cout << "Coroutine stage 1 ,co_yield" << std::endl;
//	co_yield 101;
//	std::cout << "Coroutine stage 2 ,co_yield" << std::endl;
//	co_yield 202;
//	std::cout << "Coroutine stage 3 ,co_yield" << std::endl;
//	co_yield 303;
//	std::cout << "Coroutine stage end, co_return" << std::endl;
//	co_return 808;
//}
//
//void testCoroutine()
//{
//	bool done = false;
//	std::cout << "Start coroutine_7in7out ()\n";
//	//调用协程,得到返回值c_r，后面使用这个返回值来管理协程。
//	auto c_r = coroutine_7in7out();
//	//第一次停止因为initial_suspend 返回的是suspend_always
//	//此时没有进入Stage 1
//	std::cout << "Coroutine " << (done ? "is done " : "isn't done ")
//		<< "ret =" << c_r.get() << std::endl;
//	done = c_r.move_next();
//	//此时是，co_await std::suspend_always{}
//	std::cout << "Coroutine " << (done ? "is done " : "isn't done ")
//		<< "ret =" << c_r.get() << std::endl;
//	done = c_r.move_next();
//	//此时打印Stage 1
//	std::cout << "Coroutine " << (done ? "is done " : "isn't done ")
//		<< "ret =" << c_r.get() << std::endl;
//	done = c_r.move_next();
//	std::cout << "Coroutine " << (done ? "is done " : "isn't done ")
//		<< "ret =" << c_r.get() << std::endl;
//	done = c_r.move_next();
//	std::cout << "Coroutine " << (done ? "is done " : "isn't done ")
//		<< "ret =" << c_r.get() << std::endl;
//	done = c_r.move_next();
//	std::cout << "Coroutine " << (done ? "is done " : "isn't done ")
//		<< "ret =" << c_r.get() << std::endl;
//}​

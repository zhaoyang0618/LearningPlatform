#include "Cpp20.h"

#include <vector>
#include <list>
#include <deque>
#include <ranges>
#include <algorithm>
#include <format>
#include <iostream>
#include <string_view>
#include <iomanip>
#include <iterator>

namespace stdr = std::ranges;
namespace stdrv = std::ranges::views;

// https://en.cppreference.com/w/cpp/ranges/lazy_split_view
// P2210R2: a temporary patch until online g++ >= 12
#define lazy_split_view split_view
#define lazy_split split

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

void test()
{
	std::cout << "C++ " << std::setfill('0') << std::setw(2) << cpp_year % 100 << '\n';
	std::cout << "C++ " << std::setw(2) << (__cplusplus / 100) % 100 << '\n';
}

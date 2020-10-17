#include "STLCookbookDemos.h"

//implicit template type deduction
//在使用的时候可以不指定模板参数的类型
template <typename T>
struct sum {
	T value;
	template <typename ... Ts>
	sum(Ts&& ... values) : value{ (values + ...) } {}//这里使用了折叠表达式(fold expression)
};

//
//推断指引语法
//explicit deduction guide
//Deduction Guides的基本形式如下。
//deduction - guide:
//explicit(opt) template - name(parameter - declaration - clause)->simple - template - id;
//explicit关键字是可选的关键字。simple - template - id 是模板的特化名，上面的例子中为S<typename U::type>。
//有几个对于Deduction Guides的基本要求：
//template - name应该和simple - template - id有相同的标识符。
//Deduction Guides应该和与其关联的模板有相同的作用域，如果是一个成员类模板，则还要求有相同的访问权限。
//同一个转换单元里的两个Deduction Guide声明不得有相同的parameter - declaration - clauses。
template <typename ... Ts>
sum(Ts&& ... ts)->sum<std::common_type_t<Ts...>>;

//C++17
//constexpr if
template <typename T>
class addable
{
	T val;
public:
	addable(T v) : val{ v } {}
	template <typename U>
	T add(U x) const {
		if constexpr (std::is_same_v<T, std::vector<U>>) {
			auto copy(val);
			for (auto& n : copy) {
				n += x;
			}
			return copy;
		}
		else {
			return val + x;
		}
	}
};

//C++17之前
//template <typename T>
//class addable
//{
//	T val;
//public:
//	addable(T v) : val{ v } {}
//	template <typename U>
//	std::enable_if_t<!std::is_same<T, std::vector<U>>::value, T>
//		add(U x) const { return val + x; }
//	template <typename U>
//	std::enable_if_t<std::is_same<T, std::vector<U>>::value,
//		std::vector<U>>
//		add(U x) const {
//		auto copy(val);
//		for (auto& n : copy) {
//			n += x;
//		}
//		return copy;
//	}
//};


template <typename U>
void print(U x) {
	if constexpr (std::is_class_v<U>)
	{
		if constexpr (std::is_same_v<U, std::vector<typename U::value_type>>) {
			for (auto& n : x) {
				std::cout << n << "; ";
			}
			std::cout << std::endl;
		}
		else {
			std::cout << x << std::endl;
		}
	}
	else
	{
		std::cout << x << std::endl;
	}
}


//折叠表达式
template <typename ... Ts>
auto sum2(Ts ... ts)
{
	return (ts + ...);
}

//helper函数1
template <typename R, typename ... Ts>
auto matches(const R& range, Ts ... ts)
{
	return (std::count(std::begin(range), std::end(range), ts) + ...);
}

//helper函数2 插入是否都成功
template <typename T, typename ... Ts>
bool insert_all(T& set, Ts ... ts)
{
	return (set.insert(ts).second && ...);
}

template <typename T, typename ... Ts>
bool within(T min, T max, Ts ...ts)
{
	return ((min <= ts && ts <= max) && ...);
}

template <typename T, typename ... Ts>
void insert_all(std::vector<T>& vec, Ts ... ts)
{
	(vec.push_back(ts), ...);
}


void testSTLCookbook()
{
	std::cout << "C++ 17 STL Cook book" << std::endl;
	//注意这里sum没有指定模板参数，编译器自动推导
	sum s{ 1u, 2.0, 3, 4.0f };
	sum string_sum{ std::string{"abc"}, "def" };
	std::cout << s.value << '\n'
		<< string_sum.value << '\n';

	//constexpr if用法
	auto a1 = addable<int>{1}.add(2); // is 3
	print(a1);
	auto a2 = addable<float>{1.0}.add(2); // is 3.0
	auto a3 = addable<std::string>{"aa"}.add("bb"); // is "aabb"
	std::vector<int> v{ 1, 2, 3 };
	auto a4 = addable<std::vector<int>>{v}.add(10);
	// is std::vector<int>{11, 12, 13}
	std::vector<std::string> sv{ "a", "b", "c" };
	auto a5 = addable<std::vector<std::string>>{sv}.add(std::string{ "z" });
	// is {"az", "bz", "cz"}
	print(a5);

	//fold expression
	//C++17 supports folding parameter packs with
	//the following binary operators : +, -, *, / , %, ^, &, | , =, <, >, << , >> , +=, -=, *=, /=, %=, ^=, &=,
	//|=, <<=, >>=, == , != , <= , >= , &&, || , , , .*, ->*.
	//By the way, in our example code, it does not matter if we write(ts + ...) or (... + ts);
	//both work.However, there is a difference that may be relevant in other cases--if the ... dots
	//are on the right - hand side of the operator, the fold is called a right fold.If they are on the lefthand
	//side, it is a left fold.
	int the_sum{ sum2(1, 2, 3, 4, 5) }; // Value: 15
	std::string a{ "Hello " };
	std::string b{ "World" };
	std::cout << sum2(a, b) << '\n'; // Output: Hello World
	//
	std::vector<int> v1{ 1, 2, 3, 4, 5 };
	auto cnt = matches(v1, 2, 5); // returns 2
	cnt = matches(v1, 100, 200); // returns 0
	cnt = matches("abcdefg", 'x', 'y', 'z'); // returns 0
	cnt = matches("abcdefg", 'a', 'd', 'f'); // returns 3
	//
	std::set<int> my_set{ 1, 2, 3 };
	insert_all(my_set, 4, 5, 6); // Returns true
	insert_all(my_set, 7, 8, 2); // Returns false, because the 2 collides

	//Note that if we try to insert, for example, three elements, but the second element can
	//already not be inserted, the && ... fold will short-circuit and stop inserting all the other elements :
	insert_all(my_set, 4, 2, 5); // Returns false
	// set contains {1, 2, 3, 4} now, without the 5!

	//
	within(10, 20, 1, 15, 30); // --> false
	within(10, 20, 11, 12, 13); // --> true
	within(5.0, 5.5, 5.1, 5.2, 5.3); // --> true
	//
	std::string aaa{ "aaa" };
	std::string bcd{ "bcd" };
	std::string def{ "def" };
	std::string zzz{ "zzz" };
	within(aaa, zzz, bcd, def); // --> true
	within(aaa, def, bcd, zzz); // --> false
	//
	std::vector<int> v2{ 1, 2, 3 };
	insert_all(v2, 4, 5, 6);

}

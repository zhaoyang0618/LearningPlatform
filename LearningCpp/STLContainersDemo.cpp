#include "STLContainersDemo.h"

template<typename T>
void print(const std::vector<T>& v)
{
	std::cout << "size: " << v.size() << " capacity: " << v.capacity() << std::endl;
	for (auto i : v) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;
}

struct billionaire {
	std::string name;
	double dollars;
	std::string country;
};

/// <summary>
/// 在map中插入数据存在两种情形
/// 1: 不存在关键字，插入
/// 2: 存在关键字，修改
/// C++17有新的函数可以提升一点点性能
/// </summary>
void inserting_items_efficiently_and_conditionally()
{
	std::list<billionaire> billionaires{
	 {"Bill Gates", 86.0, "USA"},
	 {"Warren Buffet", 75.6, "USA"},
	 {"Jeff Bezos", 72.8, "USA"},
	 {"Amancio Ortega", 71.3, "Spain"},
	 {"Mark Zuckerberg", 56.0, "USA"},
	 {"Carlos Slim", 54.5, "Mexico"},
	 // ...
	 {"Bernard Arnault", 41.5, "France"},
	 // ...
	 {"Liliane Bettencourt", 39.5, "France"},
	 // ...
	 {"Wang Jianlin", 31.3, "China"},
	 {"Li Ka-shing", 31.2, "Hong Kong"}
	};

	std::map<std::string, std::pair<const billionaire, size_t>> m;
	for (const auto& b : billionaires) {
		//C++17的语法: 结构化绑定
		//try_emplace也是C++17新增的
		//insert和emplace也可以完成操作，只是在已经存在的时候，也会构造对象，存在细微的性能损失
		//unordered_map也有类似的方法
		auto [iterator, success] = m.try_emplace(b.country, b, 1);
		if (!success) {
			iterator->second.second += 1;
		}

	}
	for (const auto& [key, value] : m) {
		const auto& [b, count] = value;
		std::cout << b.country << " : " << count
			<< " billionaires. Richest is "
			<< b.name << " with " << b.dollars
			<< " B$\n";
	}
}

/// <summary>
/// map的insert函数, insert_it提供的了一个位置的hint，可以降低查找时间,提升性能
/// It is important to know, that before C++11, insertion hints were considered
/// correct when they pointed before the position of the newly inserted item.
/// </summary>
void recipe_new_insertion_hint_semantics()
{
	std::map<std::string, size_t> m1{ {"b", 1}, {"c", 2}, {"d", 3} };
	auto insert_it(std::end(m1));
	for (const auto& s : { "z", "y", "x", "w" }) {
		insert_it = m1.insert(insert_it, { s, 1 });
	}
	m1.insert(std::end(m1), { "a", 1 });
	for (const auto& [key, value] : m1) {
		std::cout << "\"" << key << "\": " << value << ", ";
	}
	std::cout << '\n';
}

/// <summary>
/// 修改map的key，原则上不应该修改map的key
/// 事实上也确实不能，只能先删除，然后在insert
/// Prior to C++17, we had to remove the items of which we need to change the key value from
/// the tree, in order to reinsert them.The downside of this approach is that this always
/// needlessly reallocates some memory, which sounds bad in terms of performance.
/// Since C++17, we can removeand reinsert map nodes without any reallocation of memory.
/// We will see how that works in this recipe.
/// </summary>
template <typename M>
void print(const M& m)
{
	std::cout << "Race placement:\n";
	for (const auto& [placement, driver] : m) {
		std::cout << placement << ": " << driver << '\n';
	}
}
void modifying_the_keys_of_map()
{
	std::map<int, std::string> race_placement{
		{1, "Mario"}, {2, "Luigi"}, {3, "Bowser"},
		{4, "Peach"}, {5, "Yoshi"}, {6, "Koopa"},
		{7, "Toad"}, {8, "Donkey Kong Jr."}
	};
	print(race_placement);

	{
		auto a(race_placement.extract(3));
		auto b(race_placement.extract(8));
		std::swap(a.key(), b.key());
		race_placement.insert(move(a));
		race_placement.insert(move(b));
	}
	
	print(race_placement);
}

/// <summary>
/// 自定义类型如何使用unordered_map
/// map类要求key有一个顺序（全序）
/// 而很多类型是不存在这个顺序的，例如多维向量
/// 此时unordered_map就是一个何时的选择，它对于key的要求是两点
/// 1: hash function
/// 2: equal ==
/// </summary>
struct coord {
	int x;
	int y;
};
//2:equal:重载==运算符
bool operator==(const coord& l, const coord& r)
{
	return l.x == r.x && l.y == r.y;
}
//1:hash
//需要打开std命名控件，然后特化std::hash
namespace std
{
	template <>
	struct hash<coord>
	{
		using argument_type = coord;
		using result_type = size_t;
		result_type operator()(const argument_type& c) const
		{
			return static_cast<result_type>(c.x)
				+ static_cast<result_type>(c.y);
		}
	};
}

void using_unordered_map_with_custom_types()
{
	std::unordered_map<coord, int> m{ {{0, 0}, 1}, {{0, 1}, 2},
		{{2, 1}, 3} };
	for (const auto& [key, value] : m) {
		std::cout << "{(" << key.x << ", " << key.y
			<< "): " << value << "} ";
	}
	std::cout << '\n';
}

/// <summary>
/// 使用stack实现一个RPN(reverse polish notation)
/// (1+2)*3: 1 2 + 3 *
/// </summary>
/// <typeparam name="IT"></typeparam>
/// <param name="it"></param>
/// <param name="end"></param>
/// <returns></returns>
template <typename IT>
double evaluate_rpn(IT it, IT end)
{
	std::stack<double> val_stack;
	auto pop_stack([&]() {
		auto r(val_stack.top());
		val_stack.pop();
		return r;
		});
	std::map<std::string, double (*)(double, double)> ops{
		{"+", [](double a, double b) { return a + b; }},
		{"-", [](double a, double b) { return a - b; }},
		{"*", [](double a, double b) { return a * b; }},
		{"/", [](double a, double b) { return a / b; }},
		{"^", [](double a, double b) { return pow(a, b); }},
		{"%", [](double a, double b) { return fmod(a, b); }},
	};

	//
	for (; it != end; ++it) {
		std::stringstream ss{ *it };
		if (double val; ss >> val) {
			val_stack.push(val);
		}
		else {
			const auto r{ pop_stack() };
			const auto l{ pop_stack() };

			try {
				const auto& op(ops.at(*it));
				const double result{ op(l, r) };
				val_stack.push(result);
			}
			catch (const std::out_of_range&) {
				throw std::invalid_argument(*it);
			}
		}
	}
	return val_stack.top();
}
void impl_RPN_with_stack()
{
	std::cout << "使用stack实现一个建议的RPN" << std::endl;
	try {
		//std::cout << evaluate_rpn(std::istream_iterator<std::string>{std::cin}, {})
		//	<< '\n';

		std::stringstream s{ "3 2 1 + * 2 /" };
		std::cout << evaluate_rpn(std::istream_iterator<std::string>{s}, {}) << '\n';
		std::vector<std::string> v{ "3", "2", "1", "+", "*", "2", "/" };
		std::cout << evaluate_rpn(begin(v), end(v)) << '\n';

	}
	catch (const std::invalid_argument& e) {
		std::cout << "Invalid operator: " << e.what() << '\n';
	}
}

void testSTLContainers()
{
	std::cout << "测试STL Conatiners" << std::endl;
	//Using the erase-remove idiom on std::vector
	std::vector<int> v{ 1, 2, 3, 2, 5, 2, 6, 2, 4, 8 };
	print(v);
	//删除列表中的元素2
	const auto new_end (remove(begin(v), end(v), 2));
	print(v);
	//Interestingly, that was only one of the two steps. The vector still has the same size.The next line makes it actually shorter.
	v.erase(new_end, end(v));
	print(v);

	//
	const auto odd([](int i) { return i % 2 != 0; });
	v.erase(std::remove_if(std::begin(v), std::end(v), odd), std::end(v));
	print(v);
	//下面的操作会引发内存的分配，以及数据的移动
	v.shrink_to_fit();
	print(v);

	//
	inserting_items_efficiently_and_conditionally();

	//map的新的insert函数，加了一个参数hint，可以提升查找性能
	recipe_new_insertion_hint_semantics();

	//
	modifying_the_keys_of_map();

	//
	using_unordered_map_with_custom_types();

	//
	impl_RPN_with_stack();
}

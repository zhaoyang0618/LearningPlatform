///
/// 参考来源：
/// C++17 In Detail
/// 
#pragma once
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <variant>
#include <map>
#include <optional>
#include <any>

/// <summary>
/// 语法方面的一些内容
/// </summary>
void testMap();
void testPrinting();
void testSample();

/// <summary>
/// 规范了一些表达式计算的顺序
/// </summary>
void testStricterExpressionEvaluationOrder();

/// <summary>
/// 复制省略:可以提高性能
/// </summary>
/// <returns></returns>
int testGuaranteedCopyElision();

/// <summary>
/// 内存对齐
/// </summary>
void testDynamicMemoryAllocationforOverAlignedData();

/// 异常成为类型系统的一部分
/// You can now have two function overloads: one with noexcept and one without it.
/// 可以定义函数重载：一个有noexcept，一个没有
/// void (*p)();
/// void (**pp)() noexcept = &p; // error: cannot convert to
///							// pointer to noexcept function
/// struct S { typedef void (*p)(); operator p(); };
/// void (*q)() noexcept = S(); // error: cannot convert to
///							// pointer to noexcept

///编译出错：https://blog.csdn.net/fireflylane/article/details/84076588
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

/// 结构化绑定
/// Structured Binding Declarations
/// Such syntax is called structured binding expression.
/// 基本语法形式:
/// auto [a, b, c, ...] = expression;
/// auto [a, b, c, ...]{ expression };
/// auto [a, b, c, ...](expression);
/// 编译在背后可能做了类似如下的操作：
/// auto tempTuple = expression;
/// using a = tempTuple.first;
/// using b = tempTuple.second;
/// using c = tempTuple.third;
/// Conceptually, the expression is copied into a tuple-like object (tempTuple) with member variables
/// that are exposed through a, band c.However, the variables a, band c are not references, they are
/// aliases(or bindings) to the hidden object member variables.The temporary object has a unique name
/// assigned by the compiler.
/// 一些变形
/// const modifiers（常量）:
///		const auto [a, b, c, ...] = expression;
/// References (引用):
///		auto& [a, b, c, ...] = expression;
///		auto&& [a, b, c, ...] = expression;
/// You can also add [[attribute]]:
///		[[maybe_unused]] auto& [a, b, c, ...] = expression;
/// Structured Binding Declaration cannot be declared constexpr
/// constexpr auto [x, y] = std::pair(0, 0);//这个无法通过编译: structured binding declaration cannot be 'constexpr'
/// 有可能在C++20中允许
/// 允许进行结构化绑定的：
/// 
void testStructuredBindingExpression();

///
/// Init Statement for if and switch
/// 语法: if (init; condition) and switch (init; condition).
/// In the init section you can specify a new variable and then check it in the condition section. 
/// The variable is visible only in if / else scope.
void testInitStatementforifandswitch();

///
/// Inline Variables
/// With Non-Static Data Member Initialisation introduced in C++11, you can now declare and initialise member variables in one place :
/// Still, with static variables (or const static) you usually need to define it in some cpp file.
/// C++11 and constexpr keyword allow you to declare and define static variables in one place, but
/// it’s limited to constant expressions only.
/// Previously, only methods / functions could be specified as inline, but now you can do the same with
/// variables, inside a header file.
class User
{
	//声明的地方进行初始化: 
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

//Also, note that constexpr variables are inline implicitly, so there’s no need to use constexpr inline myVar = 10; .
//An inline variable is also more flexible than a constexpr variable as it doesn’t have to be initialised
//with a constant expression.For example, you can initialise an inline variable with rand(), but it’s
//not possible to do the same with constexpr variable.

//甚至直接像下面这样
struct MyClass2
{
	inline static const int sValue = 777;
};

//向下面的代码MyClass3可以被替换为MyClass4
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
/// C++11引入lambda表达式和常量表达式(const expression, 通过constexpr)
/// C++17中两者可以结合了
/// 标准： 10.1.5 The constexpr specifier [dcl.constexpr]
/// The definition of a constexpr function shall satisfy the following requirements:
/// * it shall not be virtual (13.3);
/// * its return type shall be a literal type; A > -each of its parameter types shall be a literal type;
///		A > -its function - body shall be = delete, = default, or a compound - statement that does not
///		contain:
///			– an asm - definition,
///			– a goto statement,
///			– an identifier label(9.1),
///			– a try - block, or
///			– a definition of a variable of non - literal type or of static or thread storage duration or for
///				which no initialisation is performed.
void testConstexprLambda();

///
/// Nested Namespaces
/// 可以用更紧凑的方式书写命名空间，和C#, Java更接近
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
//换一种更紧凑方式如下：
//namespace MySuperCompany::SecretProject::SafetySystem {
//	class SuperArmor {
//		// ...
//	};
//	class SuperShield {
//		// ...
//	};
//}

///
/// 模板
/// 
void testTemplate();

template<typename T>
struct MyType
{
	T str;
};

//自定义Deduction guide
// custom deduction guide
MyType(const char*)->MyType<std::string>;

//Another example of custom deduction guide is overload :
template<class... Ts>
struct overload : Ts... { using Ts::operator()...; };
template<class... Ts>
overload(Ts...)->overload<Ts...>; // deduction guide

//Fold Expressions
//C++11 引入了可变模板：variadic templates
//Still, variadic templates required some additional code when you wanted to implement ‘recursive’
//functions like sum, all.You had to specify rules for the recursion.
template<typename T1, typename... T>
auto SumCpp11(T1 s, T... ts) {
	return s + SumCpp11(ts...);
}
//C++17中可以简化
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
//批量加入数组
template<typename T, typename... Args>
void push_back_vec(std::vector<T>& v, Args&&... args)
{
	(v.push_back(std::forward<Args>(args)), ...);
}


//if constexpr
//The compile - time if for C++!
//The feature allows you to discard branches of an if statement at compile - time based on a constant
//expression condition.
template <typename T>
auto get_value(T t)
{
	if constexpr (std::is_pointer_v<T>)
		return *t;
	else
		return t;
}

template <typename Concrete, typename... Ts>
std::unique_ptr<Concrete> constructArgs(Ts&&... params)
{
	if (std::is_constructible_v<Concrete, Ts...>) // normal `if`
		return std::make_unique<Concrete>(std::forward<Ts>(params)...);
	else
		return nullptr;
}

class Test2
{
public:
	Test2(int, int) { }
};

//fixed
template <typename Concrete, typename... Ts>
std::unique_ptr<Concrete> constructArgs2(Ts&&... params)
{
	if constexpr (std::is_constructible_v<Concrete, Ts...>) // normal `if`
		return std::make_unique<Concrete>(std::forward<Ts>(params)...);
	else
		return nullptr;
}
//
//To be precise, the code in the discarded branch is not entirely removed from the compilation phase.
//Only expressions that are dependent on the template parameter used in the condition are not
//instantiated.The syntax must always be valid.
template <typename T>
void Calculate(T t)
{
	if constexpr (std::is_integral_v<T>)
	{
		// ...
		//static_assert(sizeof(int) == 100);
	}
	else
	{
		//这里和t相关的代码会被移除，不相关的仍然会编译, 所以需要保证语法正确
		execute(t);
		//strange syntax
		ri(t);
		for (int i = 0; i < 3; i++)
		{
			std::cout << "应该不会执行: " << i << std::endl;
		}

	}
}

// Template Code Simplification
// Chapter Templates/sfinae_example.cpp
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T> simpleTypeInfo(T t)
{
	std::cout << "foo<integral T> " << t << '\n';
	return t;
}
template <typename T>
std::enable_if_t<!std::is_integral_v<T>, T> simpleTypeInfo(T t)
{
	std::cout << "not integral \n";
	return t;
}

//The same thing can happen when using tag dispatching:
// Chapter Templates/tag_dispatching_example.cpp
template <typename T>
T simpleTypeInfoTagImpl(T t, std::true_type)
{
	std::cout << "foo<integral T> " << t << '\n';
	return t;
}
template <typename T>
T simpleTypeInfoTagImpl(T t, std::false_type)
{
	std::cout << "not integral \n";
	return t;
}
template <typename T>
T simpleTypeInfoTag(T t)
{
	return simpleTypeInfoTagImpl(t, std::is_integral<T>{});
}

//We can now simplify this pattern with if constexpr:
template <typename T>
T simpleTypeInfo2(T t)
{
	if constexpr (std::is_integral_v<T>)
	{
		std::cout << "foo<integral T> " << t << '\n';
	}
	else
	{
		std::cout << "not integral \n";
	}
	return t;
}

//属性
//Standard Attributes
//Attributes in C++11 and C++14
//C++11 took one big step to minimise the need to use vendor specific syntax.By introducing the
//standard format, we can move a lot of compiler - specific attributes into the universal set.
//C++11 provides a nicer format of specifying annotations over our code.
//The basic syntax is just [[attr]] or [[namespace::attr]] .
//You can use [[attr]] over almost anything : types, functions, enums, etc., etc.
//All Attributes available in C++17(前面4个在C++11/14开始引进的):
//* [[noreturn]]
//* [[carries_dependency]]
//* [[deprecated]]
//* [[deprecated("reason")]]
//* [[fallthrough]]
//* [[nodiscard]]
//* [[maybe_unused]]
//下面分别说明
//[[noreturn]] :
//A function does not return.The behaviour is undefined if the function with this attribute returns.
//* for example [[noreturn]] void terminate() noexcept;
//* functions like std::abort or std::exit are marked with this attribute.
//[[carries_dependency]] :
//Indicates that the dependency chain in release - consume std::memory_order propagates in and out
//of the function, which allows the compiler to skip unnecessary memory fence instructions.Mostly
//to help to optimise multi - threaded code and when using different memory models.
//C++14 added :
//	[[deprecated]] and [[deprecated("reason")]] :
//	Code marked with this attribute will be reported by the compiler.You can set a "reason" why
void testAttributes();
//[[fallthrough]] attribute
//Indicates that a fall - through in a switch statement is intentionaland a warning should not be issued
//for it. 见cpp文件中的例子
//[[maybe_unused]] attribute
//Suppresses compiler warnings about unused entities when they are declared with [[maybe_ -
//unused]]. 见cpp文件中的例子
//[[nodiscard]] attribute
//[[nodiscard]] can be applied to a function or a type declaration to mark the importance of the
//returned value :

//Ignore unknown attributes
//对于编译器相关的属性，无法识别的就忽略，在C++17之前可能是报错
//Using attribute namespaces without repetition
//void f() {
//	[[rpr::kernel, rpr::target(cpu, gpu)]] // repetition
//	doTask();
//}
//void f() {
//	[[using rpr:kernel, target(cpu, gpu)]]
//	doTask();
//}

///
/// 标准库的变化
/// 
///std::optional应用场景
/// If you want to represent a nullable type.
/// * Rather than using unique values(like - 1, nullptr, NO_VALUE or something)
/// * For example, a user’s middle name is optional.You could assume that an empty string
/// would work here, but knowing if a user entered something or not might be important.
/// std::optional<std::string> gives you more information.
/// Return a result of some computation(processing) that fails to produce a value
/// and is not an error.
/// For example, finding an element in a dictionary : if there’s no element under a key it’s not an error,
/// but we need to handle the situation.
/// To perform lazy - loading of resources.
/// For example, a resource type has no default constructor, and the construction is substantial.So you
/// can define it as std::optional<Resource>(and you can pass it around the system), and then load
/// if needed later.
/// To pass optional parameters into functions.
/// There’s a description from boost.optional which summarises when we should use the type :
/// From the boost::optional documentation : When to use Optional
///		It is recommended to use optional<T> in situations where there is exactly one, clear(to
/// 	all parties) reason for having no value of type T, and where the lack of value is as natural
/// 	as having any regular value of T.
/// While sometimes the decision to use optional might be blurry, it best suits the cases when the value
/// is empty, and it’s a normal state of the program.
/// 
/// When you use std::optional you’ll pay with an increased memory footprint
/// 下面是一种可能的实现
/// template <typename T>
/// class optional
/// {
/// 	bool _initialized;
/// 	std::aligned_storage_t<sizeof(t), alignof(T)> _storage;
/// public:
/// 	// operations
/// };
/// In short optional wraps your type, prepares space for it and then adds one boolean parameter. This
/// means it will extend the size of your Type according to the alignment rules.
/// Alignment rules are important as The standard defines :
///		Class template optional[optional.optional] : The contained value shall be allocated in a
///		region of the optional storage suitably aligned for the type T.
/// //sizeof(double) = 8
/// //sizeof(int) = 4
/// std::optional<double> od; // sizeof = 16 bytes
/// std::optional<int> oi; // sizeof = 8 bytes
/// While bool type usually takes only one byte, the optional type needs to obey the alignment rules,
/// and thus the whole wrapper is larger than just sizeof(YourType) + 1 byte.
/// struct Range
/// {
/// 	std::optional<double> mMin;
/// 	std::optional<double> mMax;
/// };
/// it will take up more space than when you use your custom type :
/// struct Range
/// {
/// 	bool mMinAvailable;
/// 	bool mMaxAvailable;
/// 	double mMin;
/// 	double mMax;
/// };
/// In the first case, we’re using 32 bytes!The second version is 24 bytes
/// Special case: optional<bool>and optional<T*>
/// While you can use optional on any type, you need to pay attention when trying to wrap boolean or
/// pointers.
/// std::optional<bool> ob - what does it model ? With such a construction, you have a tri - state
/// bool.So if you need it, then maybe it’s better to look for a real tri - state bool like boost::tribool.
/// 有可能引发混乱, 尽量不要使用了

void testOptional();

/// std::variant
/// 其他操作
/// * You can compare two variants of the same type:
///		- if they contain the same active alternative, then the corresponding comparison operator is called.
///		- If one variant has an "earlier" alternative then it's "less than" the variant with the next active alternative.
/// * Variant is a value type, so you can move it.
/// * std::hash on a variant is also possible.
/// wrap up (结束)
/// Here are the things to remember about std::variant:
///		* It holds one of several alternatives in a type - safe way
///		* No extra memory allocation is needed.The variant needs the size of the max of the sizes of the alternatives, plus some little extra space for knowing the currently active value
///		* By default, it initialises with the default value of the first alternative
///		* You can assess the value by using std::get, std::get_if or by using a form of a visitor
///		* To check the currently active type you can use std::holds_alternative or std::variant::index
///		* std::visit is a way to invoke an operation on the currently active type in the variant.It’s a callable object with overloads for all the possible types in the variant(s)
///		* Rarely std::variant might get into invalid state, you can check it via valueless_by_exception
/// 例子
/// 命令行
//Parsing a Command Line
//Command line might contain text arguments that could be interpreted in a few ways :
//*as an integer
//* as a floating point
//* as a boolean flag
//* as a string(not parsed)
class CmdLine
{
public:
	using Arg = std::variant<int, float, std::string>;
private:
	std::map<std::string, Arg> mParsedArgs;
public:
	explicit CmdLine(int argc, const char** argv) { ParseArgs(argc, argv); }
	std::optional<Arg> Find(const std::string& name) const;
	// ...
	void ParseArgs(int argc, const char** argv);
};
//解析配置文件 
//Parsing a Config File
//The idea comes from the previous example of a command line.In the case of a configuration file,
//we usually work with pairs of <Name, Value>.Where Value might be a different type : string,
//int, array, bool, float, etc.
//For such a use case even void* could be used to hold such an unknown type.However, this pattern
//is extremely error - prone.We could improve the design by using std::variant if we know all the
//possible types, or leverage std::any.
//State Machines （状态机）
struct DoorState
{
	struct DoorOpened {};
	struct DoorClosed {};
	struct DoorLocked {};
	using State = std::variant<DoorOpened, DoorClosed, DoorLocked>;
	void open()
	{
		m_state = std::visit(OpenEvent{}, m_state);
	}
	void close()
	{
		m_state = std::visit(CloseEvent{}, m_state);
	}
	void lock()
	{
		m_state = std::visit(LockEvent{}, m_state);
	}
	void unlock()
	{
		m_state = std::visit(UnlockEvent{}, m_state);
	}
	State m_state;
	struct OpenEvent
	{
		State operator()(const DoorOpened&) { return DoorOpened(); }
		State operator()(const DoorClosed&) { return DoorOpened(); }
		// cannot open locked doors
		State operator()(const DoorLocked&) { return DoorLocked(); }
	};
	struct CloseEvent
	{
		State operator()(const DoorOpened&) { return DoorClosed(); }
		State operator()(const DoorClosed&) { return DoorClosed(); }
		State operator()(const DoorLocked&) { return DoorLocked(); }
	};
	struct LockEvent
	{
		// cannot lock opened doors
		State operator()(const DoorOpened&) { return DoorOpened(); }
		State operator()(const DoorClosed&) { return DoorLocked(); }
		State operator()(const DoorLocked&) { return DoorLocked(); }
	};
	struct UnlockEvent
	{
		// cannot unlock opened doors
		State operator()(const DoorOpened&) { return DoorOpened(); }
		State operator()(const DoorClosed&) { return DoorClosed(); }
		// unlock
		State operator()(const DoorLocked&) { return DoorClosed(); }
	};
};

//Polymorphism （多态）
//C++本身可以使用虚函数实现多态
//Most of the time in C++ we can safely use runtime polymorphism based on a vtable approach.You
//have a collection of related types that share the same interface, and you have a well defined virtual
//method that can be invoked.
//But what if you have “unrelated” types that don’t share the same base class ? What if you’d like to
//quickly add new functionality without changing the code of the supported types ?
//With std::variant and std::visit we can build the following example :
class Triangle
{
public:
	void Render() { std::cout << "Drawing a triangle!\n"; }
};
class Polygon
{
public:
	void Render() { std::cout << "Drawing a polygon!\n"; }
};
class Sphere
{
public:
	void Render() { std::cout << "Drawing a sphere!\n"; }
};

void testVariant();

/// std::any (可以认为是安全的C语言的void*)
/// With std::optional you can represent some Type or nothing. With std::variant you can wrap
/// several variants into one entity.And C++17 gives us one more wrapper type : std::any which can
/// hold anything in a type - safe way.

void testAny();


///std::string_view
/// 
void testStringView();



///并行算法
/// 
/// 下面是STL实现了并行算法的函数
//adjacent_difference	inplace_merge				replace_copy
//adjacent_find			is_heap						replace_copy_if
//all_of				is_heap_until				replace_if
//any_of				is_partitioned				reverse
//copy					is_sorted					reverse_copy
//copy_if				is_sorted					rotate
//copy_n				is_sorted_until				rotate_copy
//count					lexicographical_compare		search
//count_if				max_element					search_n
//equal					merge						set_difference
//exclusive_scan		min_element					set_intersection
//fill					minmax_element				set_symmetric_difference
//fill_n				mismatch					set_union
//find					move						sort
//find_end				none_of						stable_partition
//find_first_of			nth_element					stable_sort
//find_if				partial_sort				swap_ranges
//find_if_not			partial_sort_copy			transform
//for_each				partition					transform_exclusive_scan
//for_each_n			partition_copy				transform_inclusive_scan
//generate				remove						transform_reduce
//generate_n			remove_copy					uninitialized_copy
//includes				remove_copy_if				uninitialized_copy_n
//inclusive_scan		remove_if					uninitialized_fill
//inner_product			replace						uninitialized_fill_n
//						unique						unique_copy

//for_each						similar to for_each except returns void
//for_each_n					applies a function object to the first n elements of a sequence
//reduce						similar to accumulate, except out of order execution to allow parallelism
//transform_reduce				transforms the input elements using a unary operation, then reduces the output out of order
//exclusive_scan				parallel version of partial_sum, excludes the i - th input element from the i - th sum, out of order execution to allow parallelism
//inclusive_scan				parallel version of partial_sum, includes the i - th input element in the i - th sum, out of order execution to allow parallelism
//transform_exclusive_scan		applies a functor, then calculates exclusive scan
//transform_inclusive_scan		applies a functor, then calculates inclusive scan
void testParallel();






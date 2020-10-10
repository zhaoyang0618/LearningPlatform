#include "Cpp17.h"
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include <numeric>
#include <functional>
#include <execution>
#include <string>
#include <array>
#include <set>
#include <shared_mutex>
#include <type_traits>
#include <complex>
#include <charconv>
#include <assert.h>

#include "TimeLost.h"

/// <summary>
/// 代码来源
/// https://devblogs.microsoft.com/cppblog/category/general-cpp-series/
/// </summary>
void testSample()
{
    using namespace std;

    vector<int> data(20);
    iota(begin(data), end(data), 1);
    copy(cbegin(data), cend(data), ostream_iterator<int>(cout, " "));
    cout << '\n';

    //测试sample
    //位于<algorithm>
    random_device seeder;
    const auto seed = seeder.entropy() ? seeder() : time(nullptr);
    default_random_engine generator(
        static_cast<default_random_engine::result_type>(seed));

    const size_t numberOfSamples = 5;
    vector<int> sampledData(numberOfSamples);

    for (size_t i = 0; i < 10; ++i)
    {
        sample(cbegin(data), cend(data), begin(sampledData),
            numberOfSamples, generator);
        copy(cbegin(sampledData), cend(sampledData),
            ostream_iterator<int>(cout, " "));
        cout << '\n';
    }

    //测试for_each_n: 执行前n个
    //位于<algorithm>
    for_each_n(begin(data), 5,
        [](const auto& value) { cout << value << '\n'; });

    //测试Searching，在<functional>中
    //default_searcher
    //boyer_moore_searcher
    //boyer_moore_horspool_searcher
    const string haystack = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
    const string needle = "consectetur";

    const auto result = search(cbegin(haystack), cend(haystack),
        boyer_moore_searcher(cbegin(needle), cend(needle)));

    if (result != cend(haystack))
        cout << "Found it.\n";
    else
        cout << "Not found.\n";

    //并行STL算法: 其余的参考文档:
    TimeLost lost;
    vector<double> data1(100000000);
    iota(begin(data1), end(data1), 1);

    transform(execution::par_unseq, begin(data1), end(data1), begin(data1),
        [](const auto& value) { return sqrt(value); });
    cout << "time lost: " << lost.elspsed() << endl;

    //lost.reset();
    //vector<double> data2(100000000);
    //iota(begin(data2), end(data2), 1);

    //transform(execution::seq, begin(data2), end(data2), begin(data2),
    //    [](const auto& value) { return sqrt(value); });
    //cout << "time lost: " << lost.elspsed() << endl;

    //一些工具函数
    //clamp保证返回值在范围之内
    const int low = -32'768;
    const int high = 32'767;
    cout << clamp(12'000, low, high) << '\n';
    cout << clamp(-36'000, low, high) << '\n';
    cout << clamp(40'000, low, high) << '\n';
    //最大公因数和最小公倍数
    cout << gcd(24, 44) << '\n';
    cout << lcm(24, 44) << '\n';
    //C++17 has removed one algorithm: std::random_shuffle(). 
    //This algorithm was previously already marked as deprecated by C++14. 
    //You should use std::shuffle() instead.
}

/// <summary>
/// 下面使用了一些新的语法
/// </summary>
void testMap()
{
    using namespace std;
    //C++11引入的初始化方法
    map<string, int> mapUsersAge{ {"Alex", 45}, {"John", 25} };

    //新的语法：Template Argument Deduction for Class Templates
    map mapCopy{mapUsersAge};

    //结构化绑定: Structured Bindings
    //if中的初始化: 
    //map类的新增函数
    if (auto [iter, wasAdded] = mapCopy.insert_or_assign("John", 26); !wasAdded)
    {
        cout << iter->first << " reassigned.." << endl;
    }

    for (const auto& [key, value] : mapCopy)
    {
        cout << key << ", " << value << endl;
    }
}

/// <summary>
/// constexpr
/// 可变数量模板参数
/// </summary>
void testPrinting()
{
    int i = 10;
    float f = 2.56f;
    printWithInfo(&i, &f, 30);
}

class Query
{
public:
    Query& addInt(int i)
    {
        std::cout << "addInt: " << i << '\n';
        return *this;
    }
    Query& addFloat(float f)
    {
        std::cout << "addFloat: " << f << '\n';
        return *this;
    }
};

float computeFloat()
{
    std::cout << "computing float... \n";
    return 10.1f;
}

float computeInt()
{
    std::cout << "computing int... \n";
    return 8;
}

/// <summary>
/// 更严格的计算顺序
/// </summary>
void testStricterExpressionEvaluationOrder()
{
    //C++14的时候，不能保证先执行computeFloat;后执行computeInt
    //类似这样的函数链：addFloat和addInt按顺序，但是其中的computeFloat和computeInt不保证顺序
    //C++17: a(expA).b(expB).c(expC)
    //expA is evaluated before calling b.
    Query q;
    q.addFloat(computeFloat()).addInt(computeInt());
    //Another result of this change is that when using operator overloading, the order of evaluation is
    //determined by the order associated with the corresponding built - in operator.
    //the following expressions are evaluated in the order a, then b :
    //1. a.b
    //2. a->b
    //3. a->*b
    //4. a(b1, b2, b3) // b1, b2, b3 - in any order
    //5. b @ = a // '@' means any operator
    //6. a[b]
    //7. a << b
    //8. a >> b
}

//
struct Test
{
    Test() { std::cout << "Test::Test\n"; }
    Test(const Test&) { std::cout << "Test(const Test&)\n"; }
    Test(Test&&) { std::cout << "Test(Test&&)\n"; }
    ~Test() { std::cout << "~Test\n"; }
};

Test Create()
{
    //return Test();
    Test t;
    //
    return t;
}

// based on P0135R0
struct NonMoveable
{
    NonMoveable(int x) : v(x) { }
    NonMoveable(const NonMoveable&) = delete;
    NonMoveable(NonMoveable&&) = delete;
    std::array<int, 1024> arr;
    int v;
};

NonMoveable make(int val)
{
    if (val > 0)
        return NonMoveable(val);
    return NonMoveable(-val);
}

///指定对齐
class alignas(16) vec4
{
    float x, y, z, w;
};

int testGuaranteedCopyElision()
{
    //如果不做优化，这里面Test会被多次创建，析构
    //Return Value Optimisation - RVO
    auto n = Create();
    //Named Return Value Optimisation - NRVO:
    //
    //when a temporary object is used to initialise another object(including the object returned by
    //a function, or the exception object created by a throw - expression)
    //when a variable that is about to go out of scope is returned or thrown
    //when an exception is caught by value

    //在C++14中这部分相关代码无法通过编译，因为缺少copy和move构造
    //C++17 copy elision works only for temporary objects, not for Named RVO.
    auto largeNonMoveableObj = make(90);
    return largeNonMoveableObj.v;

    //lvalue - an expression that has an identity, and which we can take the address of
    //xvalue - "eXpiring lvalue" - an object that we can move from, which we can reuse.Usually,
    //  its lifetime ends soon
    //prvalue - pure rvalue - something without a name, which we cannot take the address of, we
    //  can move from such expression
    //glvalue - "generalised" lvalue - A glvalue is an expression whose evaluation computes the
    //  location of an object, bit - field, or function
    //prvalue - "pure" rvalue - A prvalue is an expression whose evaluation initialises an object,
    //  bit - field, or operand of an operator, as specified by the context in which it appears
    //  prvalues perform initialisation, glvalues describe locations
    //The C++17 Standard specifies that when there’s a prvalue of some class or array there’s no need to
    //    create any temporary copies, the initialisation from that prvalue can happen directly. There’s no
    //    move or copy involved(so there’s no need to have a copy and move constructors); the compiler can
    //    safely do the elision.
    //It can happen :
    //    * in initialisation of an object from a prvalue : Type t = T()
    //    * in a function call where the function returns a prvalue - like in our examples.
    //There are several exceptions where the temporary is still needed :
    //    * when a prvalue is bound to a reference
    //    * when member access is performed on a class prvalue
    //    * when array subscripting is performed on an array prvalue
    //    * when an array prvalue is decayed to a pointer
    //    * when a derived-to-base conversions performed on a class prvalue
    //    * when a prvalue is used as a discarded value expression

}

///指定对齐
class alignas(16) vec5
{
    float u, x, y, z, w;
};

/// <summary>
/// 很多时候为了优化，要求申请的内存进行内存对齐
/// </summary>
void testDynamicMemoryAllocationforOverAlignedData()
{
    std::cout << "float byte: " << sizeof(float) << std::endl;
    std::cout << "double byte: " << sizeof(double) << std::endl;
    std::cout << "vec4 byte: " << sizeof(vec4) << std::endl;
    //注意下面这个是32
    std::cout << "vec5 byte: " << sizeof(vec5) << std::endl;
    //在C++11/14中，无法保证内存按照我们的要求进行对齐，
    //Often you have to use some special routines like _aligned_malloc / _aligned_free to be sure the alignment is preserved.
    //但是对于C++17
    //C++17 fixes that hole by introducing additional memory allocation functions that use align parameters:
    //void* operator new(size_t, align_val_t);
    //void* operator new[](size_t, align_val_t);
    //void operator delete(void*, align_val_t);
    //void operator delete[](void*, align_val_t);
    //void operator delete(void*, size_t, align_val_t);
    //void operator delete[](void*, size_t, align_val_t);
    //对于如下代码
    //No code changes, but now the alignment of vec4 is properly handled:
    //operator new[](sizeof(vec4), align_val_t(alignof(vec4)))
    //In other words, new is now aware of the alignment of the object.
    auto pVectors = new vec4[1000];
}

/// <summary>
/// 全部是共有变量， 可以进行结构化绑定
/// C++20中可能可以允许绑定私有变量：自然是拥有访问权限的情形，例如friend函数
/// </summary>
struct Point {
    double x;
    double y;
};

Point GetStartPoint() {
    return { 0.0, 0.0 };
}

///
/// 对于自定义类型，需要定义一些接口，才能使用结构化绑定
/// To do that you have to define get<N>, std::tuple_size and std::tuple_element specialisations for your type.
///
class UserEntry {
public:
    void Load() { }
    std::string GetName() const { return name; }
    unsigned GetAge() const { return age; }
private:
    std::string name;
    unsigned age{ 0 };
    size_t cacheEntry{ 0 }; // not exposed
};

///
/// tuple_size specifies how many fields are available, tuple_element defines the type for a specific elementand get<N> returns the values.
/// This example only allows read access of the class. If you want write access, then the class should
/// also provide accessors that return references to members. Later you have to implement get with references support.

/// 方法1: with if constexpr:
//template <size_t I> auto get(const UserEntry& u) {
//    if constexpr (I == 0) return u.GetName();
//    else if constexpr (I == 1) return u.GetAge();
//}

///
/// 方法2: Alternatively, you can also use explicit get<> specialisations rather than if constexpr:
/// 
template<std::size_t> auto get(const UserEntry&);
template<> auto get<0>(const UserEntry& u) { return u.GetName(); }
template<> auto get<1>(const UserEntry& u) { return u.GetAge(); }

namespace std {
    template <> struct tuple_size<UserEntry> : std::integral_constant<size_t, 2> { };
    template <> struct tuple_element<0, UserEntry> { using type = std::string; };
    template <> struct tuple_element<1, UserEntry> { using type = unsigned; };
}


/// <summary>
/// 对于pair或者tuple类似的内容，C++17有更好的使用方式
/// </summary>
void testStructuredBindingExpression()
{
    std::cout << "结构化绑定: " << std::endl;
    //C++11/14用法
    std::set<int> mySet;
    std::set<int>::iterator iter;
    bool inserted;
    std::tie(iter, inserted) = mySet.insert(10);
    if (inserted)
        std::cout << "Value was inserted\n";

    //C++17用法
    auto [iter2, inserted2] = mySet.insert(10);

    //注意下面的x,y是a的对应值的一个拷贝(copy)
    std::pair a(0, 1.0f);
    std::cout << a.first << ", " << a.second << std::endl;
    auto [x, y] = a;
    x = 10;
    std::cout << a.first << ", " << a.second << std::endl;
    //注意u和v是引用
    auto& [u, v] = a;
    u = 10;
    //此时a.first成了10
    std::cout << a.first << ", " << a.second << std::endl;

    //结构化绑定的情形
    // 1. works with arrays:
    // In this case an array is copied into a temporary object and a, b and c refers to copied elements from
    // the array. The number of identifier must match the number of elements in the array.
    //变量的数量必须和数组元素个数一致。
    double myArray[3] = { 1.0, 2.0, 3.0 };
    auto [a1, b1, c1] = myArray;
    // 2. If the initializer supports std::tuple_size<> and provides get<N>() and std::tuple_element functions:
    // In the above snippet we bind to myPair.But this also means that you can provide support for your
    // classes, assuming you add get<N> interface implementation.
    // 可以在自定义类中实现这些接口，从而允许结构化绑定
    std::pair myPair(0, 1.0f);
    auto [a2, b2] = myPair; // binds myPair.first/second

    // 3. If the initializer’s type contains only non static, public members:
    //The class doesn’t have to be POD, but the number of identifiers must equal the number of non-static data members.
    const auto [x1, y1] = GetStartPoint();

    //显然，把binding用于range循环是它的一个最佳应用场所之一
    const std::map<std::string, int> mapCityPopulation{
            { "Beijing", 21'707'000 },
            { "London", 8'787'892 },
            { "New York", 8'622'698 }
    };

    for (auto& [city, population] : mapCityPopulation)
        std::cout << city << ": " << population << '\n';

    //使用自定义类
    UserEntry ue;
    ue.Load();
    auto [name, age] = ue; // read access
    std::cout << name << ", " << age << '\n';
}

/// <summary>
/// 新的if和switch语句,
/// </summary>
void testInitStatementforifandswitch()
{
    const std::string myString = "My Hello World Wow";
    const auto pos = myString.find("Hello");
    if (pos != std::string::npos)
        std::cout << pos << " Hello\n";
    const auto pos2 = myString.find("World");
    if (pos2 != std::string::npos)
        std::cout << pos2 << " World\n";
    //这里使用了连个变量; 或者像下面
    //不过说实在，这个例子举得不好，为何不重复使用pos?
    //{
    //    const auto pos = myString.find("Hello");
    //    if (pos != std::string::npos)
    //        std::cout << pos << " Hello\n";
    //}
    //{
    //    const auto pos = myString.find("World");
    //    if (pos != std::string::npos)
    //        std::cout << pos << " World\n";
    //}
    //不过下面的C++17的新的语法无疑是一个很好的选择：能够尽量把变量控制在一个足够小的范围
    if (const auto pos = myString.find("Hello"); pos != std::string::npos)
        std::cout << pos << " Hello\n";
    if (const auto pos = myString.find("World"); pos != std::string::npos)
        std::cout << pos << " World\n";

    //the variable defined in the if statement is also visible in the else block
    //if中定义这个变量对else也是可见的
    if (const auto pos = myString.find("World"); pos != std::string::npos)
        std::cout << pos << " World\n";
    else
        std::cout << pos << " not found!!\n";

    //还可以使用结构化绑定 
    std::map<std::string, int> mymap{
            { "Beijing", 21'707'000 },
            { "London", 8'787'892 },
            { "New York", 8'622'698 }
    };

    // better together: structured bindings + if initializer
    if (auto [iter, succeeded] = mymap.insert({ "Beijing", 1 }); succeeded) {
        //use(iter); // ok
        // ...
    } // iter and succeeded are destroyed here
}

void testConstexprLambda()
{
    //C++11中无法通过编译
    //C++17中可以了
    auto SimpleLambda = [](int n) { return n; };
    static_assert(SimpleLambda(3) == 3, "");

    //Lambda can be also explicitly declared constexpr:
    auto SimpleLambda2 = [](int n) constexpr { return n; };
}

/// <summary>
/// 递归的起点
/// </summary>
/// <returns></returns>
auto SumCpp11() {
    return 0;
}

//模板相关的
// Declaring Non - Type Template Parameters With auto
template <auto value> void f() { }

//下面的语句在C++17可以简化
//template <typename Type, Type value> constexpr Type TConstant = value;
//constexpr auto const MySuperConst = TConstant<int, 100>;
template <auto value> constexpr auto TConstant = value;
constexpr auto const MySuperConst = TConstant<100>;
template <auto ... vs> struct HeterogenousValueList {};
using MyList = HeterogenousValueList<'a', 100, 'b'>;

//Allow typename in a template template parameters.
//Allows you to use typename instead of class when declaring a template template parameter.
//Normal type parameters can use them interchangeably, but template template parameters were
//restricted to class.
//More information in N4051.
//https://wg21.link/n4051

//Allow constant evaluation for all non-type template arguments
//Remove syntactic restrictions for pointers, references, and pointers to members that appear as nontype
//template parameters.
//More information in N4268
//https://wg21.link/n4268

//Variable templates for traits
//All the type traits that yields::value got accompanying _v variable templates.For example :
//std::is_integral<T>::value can become std::is_integral_v<T>
//std::is_class<T>::value can become std::is_class_v<T>
//This improvement already follows the _t suffix additions in C++14 (template aliases) to type traits
//that returns::type.
//More information in P0006R0
//https://wg21.link/p0006r0

//Pack expansions in using - declarations
//The feature is an enhancement for variadic templatesand parameter packs.
//The compiler will now support the using keyword in pack expansions :
//下面这个用法的含义现在完全看不懂了
//The overloaded class exposes all overloads for operator() from the base classes. Before C++17
//you would have to use recursion for parameter packs to achieve the same result. T he overloaded
//pattern is a very useful enhancement for std::visit, read more in the "Overload" section in the
//Variant chapter.
//More information in P0195
//https://wg21.link/P0195
template<class... Ts> struct overloaded : Ts... {
    using Ts::operator()...;
};

//Logical operation metafunctions
//C++17 adds handy template metafunctions:
//* template<class... B> struct conjunction; -logical AND
//* template<class... B> struct disjunction; -logical OR
//* template<class B> struct negation; -logical negation
template<typename... Ts>
std::enable_if_t<std::conjunction_v<std::is_same<int, Ts>...> >
PrintIntegers(Ts ... args)
{
    (std::cout << ... << args) << '\n';
}
//The above function PrintIntegers works with variable number of arguments, but they all have
//to be of type int.
//The helper metafunctions can increase readability of advanced template code.
//More information in P0013
//https://wg21.link/P0013

void testTemplate()
{
    std::cout << "测试模板: " << std::endl;
    //下面的make_pair是一个函数模板，可以推断出参数类型
    //所以不需要写成
    //auto myPair = std::make_pair<int, std::string>(42, "hello world");
    auto myPair = std::make_pair(42, "hello world");
    std::pair<int, std::string> myPair2(42, "hello world");
    //C++17之前对于类模板没有这样的推断功能，
    //需要按照上述要求写出pair的模板参数类型
    //C++17之后提供了类似的功能
    using namespace std::string_literals;
    std::pair myPair3(42, "hello world"s);
    std::pair myPair4(42, "hello world");
    //这样可以简化很多写法 
    //// lock guard:
    //std::shared_timed_mutex mut;
    //std::lock_guard<std::shared_timed_mutex> lck(mut);
    //// array:
    //std::array<int, 3> arr{ 1, 2, 3 };
    ///简化为
    std::shared_timed_mutex mut;
    std::lock_guard lck(mut);
    std::array arr{ 1, 2, 3 };
    //Note, that partial deduction cannot happen, you have to specify all the template parameters or none:
    //部分推到无法做到
    std::tuple t(1, 2, 3); // OK: deduction
    std::tuple<int, int, int> t2(1, 2, 3); // OK: all arguments are provided
    //下面这个无法通过编译
    //std::tuple<int> t3(1, 2, 3); // Error: partial deduction
    //很多make_Type函数可能不需要了，但是一些工厂函数还有一些额外工作, 例如make_shared
    // control block and int might be in different places in memory
    std::shared_ptr<int> p(new int{ 10 });
    // the control block and int are in the same contiguous memory section
    auto p2 = std::make_shared<int>(10);

    //Without the custom deduction T would be deduced as const char*.
    //现在会被推断为string
    MyType mt{ "Hello World" };

    //Fold Expression
    //展开为
    //auto value = 1 + (2 + (3 + 4));
    auto value = sum2(1, 2, 3, 4);
    std::cout << value << std::endl;
    //
    FoldPrint("hello", 10, 20, 30); 
    FoldSeparateLine("hello", 10, 20, 30);

    //
    std::vector<float> vf;
    push_back_vec(vf, 10.5f, 0.7f, 1.1f, 0.89f);

    //这个无法通过编译
    //需要编译期if
    //auto pTest2 = constructArgs<Test2>(10, 10, 10); // 3 args!
    auto pTest2 = constructArgs2<Test2>(10, 10, 10); // 3 args!
    if (pTest2 == nullptr)
    {
        std::cout << "failed: nullptr" << std::endl;
    }
    else
    {
        std::cout << "successed" << std::endl;
    }
    //
    Calculate(10);
    //
    simpleTypeInfo(10);
    simpleTypeInfoTag(10);
    simpleTypeInfo2(10);

    //
    f<10>(); // deduces int

    //
    PrintIntegers(1, 3, 5, 7);
}


//属性
[[deprecated("use AwesomeFunc instead")]] void GoodFunc() { }
void f() {}
void g() {}
void h() {}
static void impl1() {  } // Compilers may warn about this
[[maybe_unused]] static void impl2() {  } // Warning suppressed
void foo() {
    int x = 42; // Compilers may warn about this
    [[maybe_unused]] int y = 42; // Warning suppressed
}

[[nodiscard]] int Compute()
{
    return 0;
}

void Test4() {
    Compute(); // Warning! return value of a
    // nodiscard function is discarded
}
//这里起一个提醒作用
//If you forget to assign the result to a variable, then the compiler should emit a warning.
//What it means is that you can force users to handle errors.For example, what happens if you forget
//about using the return value from new or std::async() ?
//Additionally, the attribute can be applied to types.One use case for it might be error codes :
enum class [[nodiscard]] ErrorCode {
    OK,
    Fatal,
    System,
    FileIssue
};
ErrorCode OpenFile(std::string_view fileName);
ErrorCode SendEmail(std::string_view sendto, std::string_view text);
ErrorCode SystemCall(std::string_view text);
//Now every time you’d like to call such functions you’re "forced" to check the return value.For
//important functions checking return codes might be crucial and using [[nodiscard]] might save
//you from a few bugs.
//In the Standard, it’s defined as Discarded - value expressions so if your function is called only
//for side effects(there’s no if statement around or assignment) the compiler is encouraged to report
//a warning.
//However to suppress the warning you can explicitly cast the return value to void or use [[maybe_ -
//unused]] :
void Test5() {
    static_cast<void>(Compute()); // fine...
    [[maybe_unused]] auto ret = Compute();
}

//Attributes for namespacesand enumerators
namespace [[deprecated("use BetterUtils")]] GoodUtils {
    void DoStuff() { }
}

namespace BetterUtils {
    void DoStuff() { }
}

enum class ColorModes
{
    RGB [[deprecated("use RGB8")]],
    RGBA [[deprecated("use RGBA8")]],
    RGBA16F,
    RGB8,
    RGBA8
};

void testAttributes()
{
    //下面代码无法通过编译，原因就是上面属性说明的，已经作废
    //GoodFunc();
    char c = 'a';
    switch (c) {
    case 'a':
        f(); // Warning! fallthrough is perhaps a programmer error
    case 'b':
        g();
        [[fallthrough]]; // Warning suppressed, fallthrough is ok
    case 'c':
        h();
    }
    //With this attribute, the compiler can understand the intentions of a programmer.It’s also much more
    //    readable than using a comment.

    //下面代码无法通过编译，原因在于属性标记为作废
    //GoodUtils::DoStuff();
    //auto colMode = ColorModes::RGBA;
}

///
/// 下面的示例和C++标准库相关
//std::optional
// UI class...
//std::optional<std::string> FindUserNick()
//{
//    if (IsNickAvailable())
//        return mStrNickName; // return a string
//    return std::nullopt; // same as return { };
//}

class UserName
{
public:
    UserName() : mName("Default")
    {
        std::cout << "UserName: " << mName << std::endl;
    }

    explicit UserName(std::string str) : mName(std::move(str))
    {
        std::cout << "UserName::UserName('" << mName << "')\n";
    }

    UserName(const UserName& rhs) noexcept
        : mName(rhs.mName)
    {
        std::cout << "UserName(copy): " << mName << std::endl;
    }

    UserName(UserName&& rhs) noexcept
        : mName(std::move(rhs.mName))
    {
        std::cout << "UserName(move): " << mName << std::endl;
    }

    ~UserName()
    {
        std::cout << "UserName::~UserName (" << mName << ")" << std::endl;
    }

    UserName& operator=(const UserName& rhs)
    {
        if (this != &rhs)
        {
            mName = rhs.mName;
        }

        std::cout << "UserName::operator=" << mName << std::endl;
        return *this;
    }

private:
    std::string mName;
};

struct Point1
{
    Point1(int a, int b) : x(a), y(b) { 
        std::cout << "constructor" << std::endl;
    }

    Point1(const Point1& rhs) noexcept
        : x(rhs.x)
        , y(rhs.y)
    {
        std::cout << "Point(copy): " << std::endl;
    }

    Point1(Point1&& rhs) noexcept
        : x(rhs.x)
        , y(rhs.y)
    {
        std::cout << "Point(move): " << std::endl;
    }

    ~Point1()
    {
        std::cout << "destructor" << std::endl;
    }
    int x;
    int y;
};

//尽量使用这个
///返回nullopt
//std::optional<std::string> TryParse(Input input)
//{
//    if (input.valid())
//        return input.asString();
//    return std::nullopt;
//}

//或者像下面这样
//std::optional<std::string> TryParse(Input input)
//{
//    std::optional<std::string> oOut; // empty
//    if (input.valid())
//        oOut = input.asString();
//    return oOut;
//}


/// <summary>
/// 如果存在就返回某个值
/// 否则返回std::nullopt
/// </summary>
/// <param name="has"></param>
/// <returns></returns>
std::optional<int> Query(bool has)
{
    if (has)
        return 0;

    return std::nullopt;
}

//Be Careful With Braces when Returning
//According to the Standard if you wrap a return value into braces{} then you prevent move
//operations from happening.The returned object will be copied only.
std::optional<std::string> CreateString()
{
    std::string str{ "Hello Super Awesome Long String" };
    return { str }; // this one will cause a copy
    // return str; // this one moves
}

std::unique_ptr<int> foo1()
{
    std::unique_ptr<int> p;
    return { p }; // uses copy of unique_ptr and so it breaks...
    // return p; // this one moves, so it's fine with unique_ptr
}

//标准
//In the following copy - initialisation contexts, a move operation might be used instead of a copy
//operation : -If the expression in a return statement([stmt.return]) is a(possibly parenthesised) idexpression
//that names an object with automatic storage duration declared in the body or parameterdeclaration - clause
//of the innermost enclosing function or lambda - expression, or -if the operand of
//a throw - expression is the name of a non - volatile automatic object(other than a function or catchclause
//    parameter) whose scope does not extend beyond the end of the innermost enclosing try - block
//    (if there is one),

//Probably the most important operation for optional(apart from creation) is the way you can fetch
//the contained value.
//There are several options :
//*operator*and operator-> - if there’s no value the behaviour is undefined!
//*value() - returns the value, or throws std::bad_optional_access
//* value_or(defaultVal) - returns the value if available, or defaultVal otherwise
//To check if the value is present you can use the has_value() method or just check if (optional)
//as optional is contextually convertible to bool.
// by operator*
// compute string function:
std::optional<std::string> maybe_create_hello()
{
    return std::nullopt;
}

//
//If you have an existing optional object, then you can quickly change the contained value by using
//several operations like emplace, reset, swap, assign.If you assign(or reset) with a nullopt then
//if the optional contains a value its destructor will be called.

//示例
class UserRecord
{
public:
    UserRecord(std::string name, std::optional<std::string> nick, std::optional<int> age)
        : mName{ move(name) }, mNick{ move(nick) }, mAge{ age }
    {
    }
    friend std::ostream& operator << (std::ostream& stream, const UserRecord& user);
private:
    std::string mName;
    std::optional<std::string> mNick;
    std::optional<int> mAge;
};

std::ostream& operator << (std::ostream& os, const UserRecord& user)
{
    os << user.mName;
    if (user.mNick)
        os << ' ' << *user.mNick;
    if (user.mAge)
        os << ' ' << "age of " << *user.mAge;
    return os;
}

//示例
std::optional<int> ParseInt(const char* arg)
{
    try
    {
        return std::stoi(std::string(arg));
    }
    catch(...)
    {
        std::cerr << "cannot convert '" << arg << "' to int!" << std::endl;
    }

    return {};
}


void testOptional()
{
    // use:
    //std::optional<std::string> UserNick = UI->FindUserNick();
    //if (UserNick)
    //    Show(*UserNick);

    //There are several ways to create std::optional:
    //  * Initialise as empty
    //  * Directly with a value
    //  * With a value using deduction guides
    //  * By using make_optional
    //  * With std::in_place
    //  * From other optional
    // empty:
    std::optional<int> oEmpty;
    std::optional<float> oFloat = std::nullopt;
    // direct:
    std::optional<int> oInt(10);
    std::optional oIntDeduced(10); // deduction guides
    // make_optional
    auto oDouble = std::make_optional(3.0);
    auto oComplex = std::make_optional<std::complex<double>>(3.0, 4.0);
    // in_place
    std::optional<std::complex<double>> o7{ std::in_place, 3.0, 4.0 };
    // will call vector with direct init of {1, 2, 3}
    std::optional<std::vector<int>> oVec(std::in_place, { 1, 2, 3 });
    // copy from other optional:
    auto oIntCopy = oInt;

    //in_place Construction
    //std::optional<UserName> u0; // empty optional
    //std::optional<UserName> u1{}; // also empty
    // optional with default constructed object:
    //std::optional<UserName> u2{ UserName() };
    std::optional<UserName> opt{ std::in_place };
    //std::optional<Point1> opt1{ Point1(0, 0)};
    std::optional<Point1> opt2{ std::in_place, 0, 0 };
    auto pp = foo1();

    //
    std::optional<int> oint = 10;
    std::cout << "oint " << *oint << '\n';
    // by value()
    std::optional<std::string> ostr("hello");
    try
    {
        std::cout << "ostr " << ostr.value() << '\n';
    }
    catch (const std::bad_optional_access& e)
    {
        std::cout << e.what() << '\n';
    }
    // by value_or()
    std::optional<double> odouble; // empty
    std::cout << "odouble " << odouble.value_or(10.0) << '\n';

    // 最合适的应该首先判断是否存在，然后读取数据
    if (auto ostr = maybe_create_hello(); ostr)
        std::cout << "ostr " << *ostr << '\n';
    else
        std::cout << "ostr is null\n";

    //
    std::optional<UserName> oEmpty1;
    // emplace:
    oEmpty1.emplace("Steve");
    // calls ~Steve and creates new Mark:
    oEmpty1.emplace("Mark");
    // reset so it's empty again
    oEmpty1.reset(); // calls ~Mark
    // same as:
    //oEmpty = std::nullopt;
    // assign a new value:
    oEmpty1.emplace("Fred");
    oEmpty1 = UserName("Joe");

    //std::optional allows you to compare contained objects almost "normally", but with a few
    //exceptions when the operands are nullopt.See below :
    //When operands contain values(of the same type), then you’ll see the expected results.But when
    //one operand is nullopt then it’s always "less" than any optional with some value.
    std::optional<int> oEmpty2;
    std::optional<int> oTwo(2);
    std::optional<int> oTen(10);
    std::cout << std::boolalpha;
    std::cout << (oTen > oTwo) << '\n';
    std::cout << (oTen < oTwo) << '\n';
    std::cout << (oEmpty2 < oTwo) << '\n';
    std::cout << (oEmpty2 == std::nullopt) << '\n';
    std::cout << (oTen == 10) << '\n';

    //
    UserRecord tim{ "Tim", "SuperTim", 16 };
    UserRecord nano{ "Nathan", std::nullopt, std::nullopt };
    std::cout << tim << '\n';
    std::cout << nano << '\n';

    //
    auto oFirst = ParseInt("1");
    auto oSecond = ParseInt("2");
    if (oFirst && oSecond)
    {
        std::cout << "sum of " << *oFirst << " and " << *oSecond;
        std::cout << " is " << *oFirst + *oSecond << '\n';
    }

    //下面代码虽然合法，但是还是尽量不要这样使用,毕竟指针本身就是nullable
    std::optional<int*> opi{ new int(10) };
    if (opi && *opi)
    {
        std::cout << **opi << std::endl;
        delete* opi;
    }
    if (opi)
        std::cout << "opi is still not empty!";
}

/// <summary>
/// This is a type-safe union - you can store different type variants with the proper object lifetime guarantee.
/// The new type offers a huge advantage over the C - style union.You can store all of the types inside - no matter if it’s
/// something simple like int, or float, but also complex entities like std::vector<std::string>.
/// In all of the cases, objects will be correctly initialisedand cleaned up.
/// What’s crucial is the fact that the new type enhances implementation of design patterns.For
/// example, you can now use a visitor, pattern matchingand runtime polymorphism for unrelated
/// type hierarchies in a much easier way.
/// </summary>
///
/// union SuperFloat
/// {
///     float f;
///     int i;
/// };
///
/// int RawMantissa(SuperFloat f)
/// {
///     return f.i & ((1 << 23) - 1);
/// }
/// 
/// int RawExponent(SuperFloat f)
/// {
///     return (f.i >> 23) & 0xFF;
/// }
/// However, while the above code might work in C99, due to stricter aliasing rules it’s undefined
/// behaviour in C++!
/// There’s an existing Core Guideline Rule on that :
/// C.183 : Don’t use a union for type punning
///     Reason It is undefined behaviour to read a union member with a different type from the
///     one with which it was written.Such punning is invisible, or at least harder to spot than
///     using a named cast.Type punning using a union is a source of errors.
/// Before C++17, if you wanted a type - safe union, you could use boost variant or another third - party
/// library.But now you have std::variant.
/// When to use
/// Unless you’re doing some low - level stuff, possibly only with simple types, then unions might be a
/// valid option.But for all other uses cases, where you need alternative types, std::variant is the
/// way to go.
/// 函数式编程的类型系统
/// It’s also worth mentioning that variant types (also called a tagged union, a discriminated union, or
/// a sum type) come from the functional language worldand Type Theory.
/// 

// used to print the currently active type
struct PrintVisitor
{
    void operator()(int i) { std::cout << "int: " << i << '\n'; }
    void operator()(float f) { std::cout << "float: " << f << '\n'; }
    void operator()(const std::string& s) { std::cout << "str: " << s << '\n'; }
};

// monostate for default initialisation:
class NotSimple
{
public:
    NotSimple(int, float) { }
};

//
class MyType2
{
public:
    MyType2() { std::cout << "MyType::MyType\n"; }
    ~MyType2() { std::cout << "MyType::~MyType\n"; }
};

class OtherType
{
public:
    OtherType() { std::cout << "OtherType::OtherType\n"; }
    ~OtherType() { std::cout << "OtherType::~OtherType\n"; }
};

//
struct MultiplyVisitor
{
    float mFactor;
    MultiplyVisitor(float factor) : mFactor(factor) { }
    void operator()(int& i) const {
        i *= static_cast<int>(mFactor);
    }
    void operator()(float& f) const {
        f *= mFactor;
    }
    void operator()(std::string&) const {
        // nothing to do here...
    }
};

//Have a look at the following example, where each variant represents an ingredientant(组成部分) we want to
//compose two of them together :
struct Pizza { };
struct Chocolate { };
struct Salami { };
struct IceCream { };

//异常处理
class ThrowingClass
{
public:
    explicit ThrowingClass(int i) { if (i == 0) throw int(10); }
    operator int() { throw int(10); }
};

//variant的应用
//错误处理
enum class ErrorCode1
{
    Ok,
    SystemError,
    IoError,
    NetworkError
};

std::variant<std::string, ErrorCode1> FetchNameFromNetwork(int i)
{
    if (i == 0)
        return ErrorCode1::SystemError;
    if (i == 1)
        return ErrorCode1::NetworkError;
    return std::string("Hello World!");
}

//命令行
CmdLine::Arg TryParseString(std::string_view sv)
{
    // try with float first
    float fResult = 0.0f;
    const auto last = sv.data() + sv.size();
    const auto res = std::from_chars(sv.data(), last, fResult);
    if (res.ec != std::errc{} || res.ptr != last)
    {
        // if not possible, then just assume it's a string
        return std::string{ sv };
    }
    // no fraction part? then just cast to integer
    if (static_cast<int>(fResult) == fResult)
        return static_cast<int>(fResult);
    return fResult;
}

void CmdLine::ParseArgs(int argc, const char** argv)
{
    // the form: -argName value -argName value
    for (int i = 1; i < argc; i += 2)
    {
        if (argv[i][0] != '-') // super advanced pattern matching! :)
            throw std::runtime_error("wrong command name");
        mParsedArgs[argv[i] + 1] = TryParseString(argv[i + 1]);
    }
}

std::optional<CmdLine::Arg> CmdLine::Find(const std::string& name) const
{
    if (const auto it = mParsedArgs.find(name); it != mParsedArgs.end())
        return it->second;
    return { };
}

/// <summary>
/// variant
/// </summary>
void testVariant()
{
    //No extra heap allocation occurs.
    //The variant class calls destructors and constructors of non-trivial types, so in the example, the
    //string object is cleaned up before we switch to new variants.
    std::variant<int, float, std::string> intFloatString;
    static_assert(std::variant_size_v<decltype(intFloatString)> == 3);

    // default initialised to the first alternative, should be 0
    std::visit(PrintVisitor{}, intFloatString);

    // index will show the currently used 'type'
    std::cout << "index = " << intFloatString.index() << std::endl;
    intFloatString = 100.0f;
    std::cout << "index = " << intFloatString.index() << std::endl;
    intFloatString = "hello super world";
    std::cout << "index = " << intFloatString.index() << std::endl;

    //try with get_if:
    if (const auto intPtr = std::get_if<int>(&intFloatString))
        std::cout << "int: " << *intPtr << std::endl;
    else if(const auto floatPtr = std::get_if<float>(&intFloatString))
        std::cout << "float: " << *floatPtr << std::endl;

    //
    if (std::holds_alternative<int>(intFloatString))
        std::cout << "the variant holds an int!" << std::endl;
    else if (std::holds_alternative<float>(intFloatString))
        std::cout << "the variant holds a float" << std::endl;
    else if (std::holds_alternative<std::string>(intFloatString))
        std::cout << "the variant holds a string" << std::endl;

    //try/catch and bad_variant_access
    try
    {
        auto f = std::get<float>(intFloatString);
        std::cout << "float! " << f << std::endl;
    }
    catch (std::bad_variant_access&)
    {
        std::cout << "our variant doesn't hold float at this moment..." << std::endl;
    }

    //创建
    //  *By default, a variant object is initialised with the first type,
    //    - if that's not possible when the type doesn't have a default constructor, then you'll get a compiler error
    //    - you can use std::monostate to pass it as the first type in that case
    //    - monostate allows you to build a variant with "no-value" so it can behave similarly to optional
    //  * You can initialise it with a value, and then the best matching type is used
    //    - if there's an ambiguity, then you can use a version std::in_place_index to explicitly mention what type should be used
    //  * std::in_place also allows you to create more complex types and pass more parameters to the constructor
    // default initialisation: (the first type has to have a default ctor)
    std::variant<int, float> intFloat;
    std::cout << intFloat.index() << ", val: " << std::get<int>(intFloat) << '\n';

    //注意这个的创建
    // std::variant<NotSimple, int> cannotInit; // error
    std::variant<std::monostate, NotSimple, int> okInit;
    std::cout << okInit.index() << '\n';
    // pass a value:
    std::variant<int, float, std::string> intFloatString1{ 10.5f };
    std::cout << intFloatString1.index()
        << ", value " << std::get<float>(intFloatString1) << std::endl;

    // ambiguity
    // double might convert to float or int, so the compiler cannot decide
    //std::variant<int, float, std::string> intFloatString1 { 10.5 };

    // ambiguity resolved by in_place
    std::variant<long, float, std::string> longFloatString{
        std::in_place_index<1>, 7.6 // double!
    };
    std::cout << longFloatString.index() << ", value "
        << std::get<float>(longFloatString) << '\n';
    // in_place for complex types
    std::variant<std::vector<int>, std::string> vecStr{
        std::in_place_index<0>, { 0, 1, 2, 3 }
    };

    std::cout << vecStr.index() << ", vector size "
        << std::get<std::vector<int>>(vecStr).size() << '\n';

    // copy-initialize from other variant:
    std::variant<int, float> intFloatSecond{ intFloat };
    std::cout << intFloatSecond.index() << ", value "
        << std::get<int>(intFloatSecond) << '\n';

    //更改
    //There are four ways to change the current value of the variant :
    //*the assignment operator
    //    * emplace
    //    * getand then assign a new value for the currently active type
    //    * a visitor(you cannot change the type, but you can change the value of the current alternative)
    //The important part is to know that everything is type safeand also that the object lifetime is
    //honoured.
    intFloatString = 10; // we're now an int
    intFloatString.emplace<2>(std::string("Hello")); // we're now string again

    // std::get returns a reference, so you can change the value:
    std::get<std::string>(intFloatString) += std::string(" World");
    intFloatString = 10.1f;
    if (auto pFloat = std::get_if<float>(&intFloatString); pFloat)
        *pFloat *= 2.0f;

    //生命周期
    std::variant<std::string, int> v{ "Hello A Quite Long String" };
    // v allocates some memory for the string
    v = 10; // we call destructor for the string!
    // no memory leak

    //
    std::variant<MyType2, OtherType> v2;
    v2 = OtherType();

    //如何访问其中元素
    //You have std::get<Type | Index>(variant) which is a non member function.It returns a
    //reference to the desired type if it's active(you can pass a Type or Index).If not then you’ll get
    //std::bad_variant_access exception.
    //The next option is std::get_if.This function is also a non - member and won't throw.It returns a
    //pointer to the active type or nullptr.While std::get needs a reference to the variant, std::get_if takes a pointer
    try
    {
        //
        auto f = std::get<float>(intFloatString);
        std::cout << "float! " << f << '\n';
    }
    catch (std::bad_variant_access&)
    {
        std::cout << "our variant doesn't hold float at this moment...\n";
    }
    //
    if (const auto intPtr = std::get_if<0>(&intFloatString))
        std::cout << "int!" << *intPtr << '\n';
    //However, probably the most important way to access a value inside a variant is by using visitors.
    //If you pass only one variant, then you have to have overloads for the types from that variant.If
    //you give two variants, then you have to have overloads for all possible pairs of the types from the variants.
    //A visitor is "a Callable that accepts every possible alternative from every variant".
    // a generic lambda:
    auto PrintVisitor = [](const auto& t) { std::cout << t << '\n'; };
    std::variant<int, float, std::string> intFloatString2{ "Hello" };
    std::visit(PrintVisitor, intFloatString2);

    //
    auto TwiceMoreVisitor = [](auto& t) { t *= 2; };
    std::variant<int, float> intFloat2{ 20.4f };
    std::visit(PrintVisitor, intFloat2);
    std::visit(TwiceMoreVisitor, intFloat2);
    std::visit(PrintVisitor, intFloat2);

    //更常见的用法是不同类型不同处理:
    std::visit(MultiplyVisitor(0.5f), intFloat2);
    std::visit(PrintVisitor, intFloat2);

    //Overload
    //参考：
    //https://wg21.link/p0051r3
    //https://www.bfilipek.com/2019/02/2lines3featuresoverload.html
    //With this utility you can write all lambdas for all matching types in one place :
    std::variant<int, float, std::string> myVariant;
    std::visit(
        overload{
        [](const int& i) { std::cout << "int: " << i << std::endl; },
        [](const std::string& s) { std::cout << "string: " << s << std::endl; },
        [](const float& f) { std::cout << "float: " << f << std::endl; }
        },
        myVariant
    );
    //Currently this helper is not a part of the Standard Library(it might be added into with C++20).You
    //can implement it with the following code :
    //template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
    //template<class... Ts> overload(Ts...)->overload<Ts...>;
    //The code creates a struct that inherits from lambdasand uses their Ts::operator().The whole
    //structure can now be passed to std::visit - it will then select the proper overload.
    //overload uses three C++17 features:
    //* Pack expansions in using declarations - shortand compact syntax with variadic templates.
    //* Custom template argument deduction rules - this allows the compiler to deduce types of
    //  lambdas that are the base classes for the pattern.Without it we’d have to define a "make" function.
    //* Extension to aggregate Initialisation - the overload pattern uses aggregate initialisation to init
    //base classes.Before C++17 it was not possible.
    std::visit(overload{
        [](int& i) { i *= 2; },
        [](float& f) { f *= 2.0f; },
        [](std::string& s) { s = s + s; }
        }, intFloatString2);
    std::visit(PrintVisitor, intFloatString2);

    //
    //Visiting Multiple Variants
    //    std::visit allows you not only to visit one variant but many in the same call.However, it’s
    //    important to know that with multiple variants you have to implement function overloads taking
    //    as many arguments as the number of input variants.And you have to provide all the possible
    //    combination of types.
    //For example for:
    std::variant<int, float, char> v3{ 's' };
    std::variant<int, float, char> v4{ 10 };
    //You have to provide 9 function overloads if you call std::visit on the two variants :
    std::visit(overload{
        [](int a, int b) {},
        [](int a, float b) {},
        [](int a, char b) {},
        [](float a, int b) {},
        [](float a, float b) {},
        [](float a, char b) {},
        [](char a, int b) {},
        [](char a, float b) {},
        [](char a, char b) {}
        }, v3, v4); 
    //If you skip one overload, then the compiler will report an error.
    //两两组合
    std::variant<Pizza, Chocolate, Salami, IceCream> firstIngredient{ IceCream() };
    std::variant<Pizza, Chocolate, Salami, IceCream> secondIngredient{ Chocolate() };
    std::visit(overload{
        [](const Pizza& p, const Salami& s) {
        std::cout << "here you have, Pizza with Salami!\n";
        },
        [](const Salami& s, const Pizza& p) {
        std::cout << "here you have, Pizza with Salami!\n";
        },
        [](const Chocolate& c, const IceCream& i) {
        std::cout << "Chocolate with IceCream!\n";
        },
        [](const IceCream& i, const Chocolate& c) {
        std::cout << "IceCream with a bit of Chocolate!\n";
        },
        [](const auto& a, const auto& b) {  //注意这个用法，generic lambda
        std::cout << "invalid composition...\n";
        },
        }, firstIngredient, secondIngredient);

    //The above code uses overloadand uses multiple lambdas rather than a separate struct with overloads for operator().
    //What’s interesting is that the example provides implementation only for “valid” ingredient compositions,
    //while the “rest” is handled by generic lambdas(from C++14).
    //A generic lambda [](const auto& a, const auto& b) { } is equivalent to the following callable type :
    //class UnnamedUniqueClass // << compiler specific name...
    //{
    //public:
    //    template<typename T, typename U>
    //    auto operator () (const T& a, const T& b) const { }
    //};
    //The generic lambda used in the example will provide all the remaining function overloads for the
    //ingredient types.Since it’s a template, it will always fall behind the concrete overload(lambda with
    //concrete types) when the best viable function is determined.

    //关于异常
    std::variant<int, ThrowingClass> v5;
    // change the value:
    try
    {
        v5 = ThrowingClass(0);
    }
    catch (...)
    {
        std::cout << "catch(...)\n";
        // we keep the old state!
        std::cout << v.valueless_by_exception() << '\n';
        std::cout << std::get<int>(v) << '\n';
    }
    // inside emplace
    try
    {
        v5.emplace<0>(ThrowingClass(10)); // calls the operator int
    }
    catch (...)
    {
        std::cout << "catch(...)\n";
        // the old state was destroyed, so we're not in invalid state!
        std::cout << v.valueless_by_exception() << '\n';
    }
    //对上述代码的分析，注意前一种和后一种是有差异的
    //In the first case -with the assignment operator - the exception is thrown in the constructor of the
    //type.This happens before the old value is replaced in the variant, so the variant state is unchanged.
    //As you can see we can still access intand print it.
    //However, in the second case -emplace - the exception is thrown after the old state of the variant is
    //destroyed.emplace calls operator int to replace the value, but that throws.After that, the variant
    //is in the wrong state, and we cannot recover the previous state.
    //Also note that a variant that is "valueless by exception" is in an invalid state.Accessing a value from
    //such variant is not possible.That’s why variant::index returns variant_npos, and std::get
    //and std::visit will throw bad_variant_access.

    //性能和内存
    //std::variant uses the memory in a similar way to union : so it will take the max size of the
    //underlying types.But since we need something that will know what the currently active alternative
    //is, then we need to use some more space.Plus everything needs to honour the alignment rules.
    std::cout << "sizeof string: "
        << sizeof(std::string) << '\n';
    std::cout << "sizeof variant<int, string>: "
        << sizeof(std::variant<int, std::string>) << '\n';
    std::cout << "sizeof variant<int, float>: "
        << sizeof(std::variant<int, float>) << '\n';
    std::cout << "sizeof variant<int, double>: "
        << sizeof(std::variant<int, double>) << '\n';
    //What’s more interesting is that std::variant won’t allocate any extra space!No dynamic
    //allocation happens to hold variants or the discriminator.

    //例子
    //Error Handling
    //    The basic idea is to wrap the possible return type with some ErrorCode, and that way allow
    //    functions to output more information about the errors.Without using exceptions or output
    //    parameters.This is similar to what std::expected - a new type planned for the future C++
    //    Standard.
    auto response = FetchNameFromNetwork(0);
    if (std::holds_alternative<std::string>(response))
        std::cout << std::get<std::string>(response) << "n";
    else
        std::cout << "Error!\n";
    response = FetchNameFromNetwork(10);
    if (std::holds_alternative<std::string>(response))
        std::cout << std::get<std::string>(response) << "n";
    else
        std::cout << "Error!\n";
    //关于多态的例子 
    std::vector<std::variant<Triangle, Polygon, Sphere>> objects{
        Polygon(),
        Triangle(),
        Sphere(),
        Triangle()
    };
    auto CallRender = [](auto& obj) { obj.Render(); };
    for (auto& obj : objects)
        std::visit(CallRender, obj);
    //上述代码实现了一种多态
    //The above example shows only the first case of invoking a method from unrelated types.It wraps
    //all the possible shape types into a single variantand then uses a visitor to dispatch the call to the
    //proper type.
    //If you’d like, for example, to sort objects, then you can write another visitor, one that holds some
    //state.And that way you’ll get more functionality without changing the types.

}

/// <summary>
/// 
/// </summary>
struct MyType3
{
    MyType3(int m = 0, int n = 0)
        : _m(m)
        , _n(n)
    {
        std::cout << "MyType::MyType3" << std::endl;
    }

    ~MyType3()
    {
        std::cout << "MyType::~MyType3" << std::endl;
    }

    void Print() { std::cout << _m << ", " << _n << '\n'; }

    int _m;
    int _n;
};

void testAny()
{
    std::cout << "测试std::any" << std::endl;
    std::any a(12);
    // set any value:
    a = std::string("Hello!");
    a = 16;
    // reading a value:
    // we can read it as int
    std::cout << std::any_cast<int>(a) << '\n';
    // but not as string:
    try
    {
        std::cout << std::any_cast<std::string>(a) << '\n';
    }
    catch (const std::bad_any_cast& e)
    {
        std::cout << e.what() << '\n';
    }

    //
    // reset and check if it contains any value:
    a.reset();
    if (!a.has_value())
    {
        std::cout << "a is empty!" << '\n';
    }

    // you can use it in a container:
    std::map<std::string, std::any> m;
    m["integer"] = 10;
    m["string"] = std::string("Hello World");
    m["float"] = 1.0f;
    for (auto& [key, val] : m)
    {
        if (val.type() == typeid(int))
            std::cout << "int: " << std::any_cast<int>(val) << '\n';
        else if (val.type() == typeid(std::string))
            std::cout << "string: " << std::any_cast<std::string>(val) << '\n';
        else if (val.type() == typeid(float))
            std::cout << "float: " << std::any_cast<float>(val) << '\n';
    }

    //There are several ways you can create std::any object :
    //    *a default initialisation - then the object is empty
    //    * a direct initialisation with a value / object
    //    * in place std::in_place_type
    //    * via std::make_any
    // default initialisation:
    std::any a1;
    assert(!a1.has_value());
    // initialisation with an object:
    std::any a2{ 10 }; // int
    std::any a3{ MyType3{10, 11} };
    // in_place:
    std::any a4{ std::in_place_type<MyType3>, 10, 11 };
    std::any a5{ std::in_place_type<std::string>, "Hello World" };
    // make_any
    std::any a6 = std::make_any<std::string>("Hello World");

    //
    std::cout << "复杂对象" << std::endl;

    //这里需要一个临时对象
    std::any a7{ UserName{"hello"} };

    //而这一个直接创建
    std::any a8{ std::in_place_type<UserName>, "hello" };
    //或者这样
    auto a9 = std::make_any<UserName>("hello");

    //更改数据
    std::cout << "更改数据" << std::endl;
    std::any a10;
    a10 = MyType3(10, 11);
    a10 = std::string("Hello");
    a10.emplace<float>(100.5f);
    a10.emplace<std::vector<int>>({ 10, 11, 12, 13 });
    a10.emplace<MyType3>(10, 11);

    //对象生命周期
    //The crucial part of being safe for std::any is not to leak any resources.To achieve this behaviour
    //std::any will destroy any active object before assigning a new value.
    std::cout << "生命周期" << std::endl;
    std::any var = std::make_any<MyType3>();
    var = 100.0f;
    std::cout << std::any_cast<float>(var) << std::endl;

    //访问
    //To read the currently active value in std::any you have mostly one option - template<class
    //ValueType> ValueType std::any_cast.This function returns the value of the requested type if
    //it's in the object.
    //However, this function template is quite powerful, as it has three modes :
    //    * read access - returns a copy of the value, and throws std::bad_any_cast when it fails
    //    * read / write access - returns a reference, and throws std::bad_any_cast when it fails
    //    * read / write access - returns a pointer to the value(const or not) or nullptr on failure
    std::cout << "访问数据" << std::endl;
    std::any var1 = std::make_any<MyType3>(10, 10);
    try
    {
        std::any_cast<MyType3&>(var1).Print();
        std::any_cast<MyType3&>(var1)._m = 11; // read/write
        std::any_cast<MyType3&>(var1).Print();
        std::any_cast<int>(var1); // throw!
    }
    catch (const std::bad_any_cast& e)
    {
        std::cout << e.what() << '\n';
    }
    int* p = std::any_cast<int>(&var1);
    std::cout << (p ? "contains int... \n" : "doesn't contain an int...\n");
    if (MyType3* pt = std::any_cast<MyType3>(&var1); pt)
    {
        pt->_m = 12;
        std::any_cast<MyType3&>(var1).Print();
    }

    //性能和内存
    //Performance& Memory Considerations
    //std::any looks quite powerful, and you might use it to hold variables of variable types… but you
    //might ask what the price is for such flexibility.
    //The main issue : extra dynamic memory allocations.
    //std::variantand std::optional don’t require any extra memory allocations but this is because
    //they know which type(or types) will be stored in the object.std::any does not know which types
    //might be storedand that’s why it might use some heap memory.
    //Will it always happen, or sometimes ? What are the rules ? Will it happen even for a simple type like
    //int ?
    //Let’s see what the standard says :
    //From The Standard :
    //    Implementations should avoid the use of dynamically allocated memory for a small
    //    contained value.Example : where the object constructed is holding only an int.Such
    //    small - object optimisation shall only be applied to types T for which is_nothrow_move_ -
    //    constructible_v<T> is true.
    //To sum up - Implementations are encouraged to use SBO - Small Buffer Optimisation.But that
    //also comes at some cost : it will make the type larger - to fit the buffer.
    //用途
    //Parsing files
    //Message Passing
    //Properties: 
    //struct property
    //{
    //    property();
    //    property(const std::string&, const std::any&);
    //    std::string name;
    //    std::any value;
    //};
    //typedef std::vector<property> properties;

    //Wrap up
    //* std::any is not a template class
    //* std::any uses Small Buffer Optimisation, so it will not dynamically allocate memory for
    //simple types like ints, doubles… but for larger types, it will use extra new.
    //* std::any might be considered ‘heavy’, but offers a lot of flexibilityand type - safety.
    //* you can access the currently stored value by using any_cast that offers a few “modes” : for
    //example it might throw an exception or return nullptr.
    //* use it when you don’t know the possible types - in other cases consider std::variant.


    std::cout << "函数结束" << std::endl;
}

///并行算法
/// 
void testParallel()
{
    std::cout << "测试并行算法" << std::endl;
    std::vector<int> vec(10);
    std::iota(vec.begin(), vec.end(), 0);
    std::vector<int> output;
    std::mutex m;
    std::for_each(std::execution::par, vec.begin(), vec.end(),
        [&output, &m](int& elem) {
            if (elem % 2 == 0) {
                std::lock_guard guard(m);
                output.push_back(elem);
            }
        });

    auto PrintVisitor = [](const auto& t) { std::cout << t << "; "; };
    std::cout << "source: " << std::endl;
    std::for_each(vec.begin(), vec.end(), PrintVisitor);
    std::cout << std::endl;
    std::cout << "output: " << std::endl;
    std::for_each(output.begin(), output.end(), PrintVisitor);
    std::cout << std::endl;

    //
    //With par_unseq function invocations might be interleaved, so it’s forbidden to use unsafe vectorised
    //code.For example, using mutexes or memory allocation might lead to data races and deadlocks.
    //Don’t use synchronisation and memory allocation when executing with par_unseq policy.
    //When using execution policies, you need to be prepared for two kinds of situations.
    //    * the scheduler, or the implementation fails to allocate resources for the invocation - then
    //    std::bad_alloc is thrown.
    //    * an exception is thrown from the user code(a functor) - in that case, the exception is not rethrown,
    //    std::terminate() is called.
    //下面这段代码的异常无法捕捉到，会直接退出
    //try
    //{
    //    std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    //    std::for_each(std::execution::par, v.begin(), v.end(),
    //        [](int& i) {
    //            std::cout << i << '\n';
    //            if (i == 5)
    //                throw std::runtime_error("something wrong... !");
    //        });
    //}
    //catch (const std::bad_alloc& e)
    //{
    //    std::cout << "Error in execution: " << e.what() << '\n';
    //}
    //catch (const std::exception& e) // will not happen
    //{
    //    std::cout << e.what() << '\n';
    //}
    //catch (...)
    //{
    //    std::cout << "error!\n";
    //}
    //If you run the above code, the catch section will only handle std::bad_alloc.And if you exit a
    //lambda because of some exception, then the std::terminate will be called.The exceptions are not re-thrown
    //When you use parallel algorithms, for better error handling try to make your functors noexcept.


    //
    std::vector<int> v1(100);
    std::iota(v1.begin(), v1.end(), 0);
    std::for_each(std::execution::par, v1.begin(), v1.end(),
        [](int& i) { i += 10; });
    std::for_each_n(std::execution::par, v1.begin(), v1.size() / 2,
        [](int& i) { i += 10; });

}



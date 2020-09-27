#include "Cpp17.h"
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include <numeric>
#include <functional>
#include <execution>
#include <map>
#include <string>
#include <array>
#include <set>
#include <shared_mutex>
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
}

#include "ThreadDemo.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <future>

///
/// https://blog.csdn.net/watson2016/article/details/52860271
/// 
int countdown(int from, int to) {
    for (int i = from; i != to; --i) {
        std::cout << i << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Finished!\n";
    return from - to;
}

void ThreadDemo::TestPackagedTask()
{
    std::packaged_task<int(int, int)> task(countdown); // 设置 packaged_task
    std::future<int> ret = task.get_future(); // 获得与 packaged_task 共享状态相关联的 future 对象.
    std::thread th(std::move(task), 10, 0);   //创建一个新线程完成计数任务.
    int value = ret.get();                    // 等待任务完成并获取结果.
    std::cout << "The countdown lasted for " << value << " seconds.\n";
    th.join();
}

void ThreadDemo::TestPackageTaskConstructor()
{
    std::packaged_task<int(int)> foo; // 默认构造函数.

   // 使用 lambda 表达式初始化一个 packaged_task 对象.
    std::packaged_task<int(int)> bar([](int x) {return x * 2; });

    foo = std::move(bar); // move-赋值操作，也是 C++11 中的新特性.

    // 获取与 packaged_task 共享状态相关联的 future 对象.
    std::future<int> ret = foo.get_future();

    std::thread(std::move(foo), 10).detach(); // 产生线程，调用被包装的任务.

    int value = ret.get(); // 等待任务完成并获取结果.
    std::cout << "The double of 10 is " << value << ".\n";
}

// 在新线程中启动一个 int(int) packaged_task.
std::future<int> launcher(std::packaged_task<int(int)>& tsk, int arg)
{
    if (tsk.valid()) {
        std::future<int> ret = tsk.get_future();
        std::thread(std::move(tsk), arg).detach();
        return ret;
    }
    else return std::future<int>();
}

void ThreadDemo::TestPackageTaskValid()
{
    std::packaged_task<int(int)> tsk([](int x) {return x * 2; });
    std::future<int> fut = launcher(tsk, 25);
    std::cout << "The double of 25 is " << fut.get() << ".\n";
}

void ThreadDemo::TestPackageTaskFuture()
{
    std::cout << "constrcutor" << std::endl;
    std::packaged_task<int(int)> tsk([](int x) 
        {
            std::cout << "start" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            return x * 3; 
        }); // package task
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "get_future" << std::endl;
    std::future<int> fut = tsk.get_future();   // 获取 future 对象.
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "thread" << std::endl;
    std::thread(std::move(tsk), 100).detach();   // 生成新线程并调用packaged_task.
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "get" << std::endl;
    int value = fut.get();                     // 等待任务完成, 并获取结果.
    std::cout << "The triple of 100 is " << value << ".\n";
}

int triple(int x) { return x * 3; }
void ThreadDemo::TestPackageTaskReset()
{
    std::packaged_task<int(int)> tsk(triple); // package task

    std::future<int> fut = tsk.get_future();
    std::thread(std::move(tsk), 100).detach();
    std::cout << "The triple of 100 is " << fut.get() << ".\n";

    // re-use same task object:
    tsk.reset();
    fut = tsk.get_future();
    std::thread(std::move(tsk), 200).detach();
    std::cout << "Thre triple of 200 is " << fut.get() << ".\n";
}

///
/// https://www.cnblogs.com/diegodu/p/6737973.html
/// 
long long factor(int n)
{
    //计算阶乘
    long long ret = 1;
    for (int i = 1; i <= n; i++)
        ret *= i;
    return ret;

}

void ThreadDemo::TestAsync()
{
    //方法1
    std::vector<int> data = { 9,11,13,17,19 };
    std::vector<std::future<long long>> fus;
    for (auto i : data)
    {
        std::packaged_task<long long() > pt(std::bind(factor, i));
        fus.push_back(std::move(pt.get_future()));
        std::thread(std::move(pt)).detach();
    }
    for (auto& i : fus)
    {
        i.wait();
        std::cout << i.get() << std::endl;
    }

    //方法2
    std::vector<std::future<long long>> fus2;
    for (auto i : data) fus2.push_back(std::async(factor, i));
    for (auto& i : fus2)
    {
        i.wait();
        std::cout << i.get() << std::endl;
    }
}

///
/// https://blog.csdn.net/fengbingchun/article/details/104127352
/// 


/// <summary>
/// https://www.cnblogs.com/bhlsheji/p/5035018.html
/// </summary>
std::mutex mtx; // 全局相互排斥锁.
std::condition_variable cv; // 全局条件变量.
bool ready = false; // 全局标志位
void do_print_id(int id)
{
    std::unique_lock <std::mutex> lck(mtx);
    while (!ready) // 假设标志位不为 true, 则等待...
        cv.wait(lck); // 当前线程被堵塞, 当全局标志位变为 true 之后,
    // 线程被唤醒, 继续往下运行打印线程编号id.
    std::cout << "thread " << id << '\n';
}

void go()
{
    std::unique_lock <std::mutex> lck(mtx);
    ready = true; // 设置全局标志位为 true.
    cv.notify_all(); // 唤醒全部线程.
}

//另一个例子
int cargo = 0;
bool shipment_available()
{
    return cargo != 0;
}

// 消费者线程.
void consume(int n)
{
    for (int i = 0; i < n; ++i) {
        std::unique_lock <std::mutex> lck(mtx);
        cv.wait(lck, shipment_available);
        std::cout << cargo << '\n';
        cargo = 0;
    }
}
void ThreadDemo::testConditionVariable()
{
    //示例1
    std::thread threads[10];
    // spawn 10 threads:
    for (int i = 0; i < 10; ++i)
        threads[i] = std::thread(do_print_id, i);

    std::cout << "10 threads ready to race...\n";
    go(); // go!

    for (auto& th : threads)
        th.join();

    //示例2
    std::thread consumer_thread(consume, 10); // 消费者线程.

    // 主线程为生产者线程, 生产 10 个物品.
    for (int i = 0; i < 10; ++i) {
        while (shipment_available())
            std::this_thread::yield();
        std::unique_lock <std::mutex> lck(mtx);
        cargo = i + 1;
        cv.notify_one();
    }

    consumer_thread.join();
}

///
///https://blog.csdn.net/Watson2016/article/details/52860797?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_title-0&spm=1001.2101.3001.4242
/// 
/// a non-optimized way of checking for prime numbers:
bool is_prime(int x)
{
    //还可以检验更少的数据sqrt(x)即可。
    auto threshold = x / 2;
    for (int i = 2; i < threshold; ++i)
        if (x % i == 0)
            return false;
    return true;
}

int do_get_value() { return 10; }

void get_int(std::promise<int>& prom) {
    int x;
    std::cout << "Please, enter an integer value: ";
    std::cin.exceptions(std::ios::failbit);   // throw on failbit
    try {
        std::cin >> x;                         // sets failbit if input is not int
        prom.set_value(x);
    }
    catch (std::exception&) {
        prom.set_exception(std::current_exception());
    }
}

void print_int(std::future<int>& fut) {
    try {
        int x = fut.get();
        std::cout << "value: " << x << '\n';
    }
    catch (std::exception& e) {
        std::cout << "[exception caught: " << e.what() << "]\n";
    }
}

double ThreadTask(int n) {
    std::cout << std::this_thread::get_id()
        << " start computing..." << std::endl;

    double ret = 0;
    for (int i = 0; i <= n; i++) {
        ret += std::sin(i);
    }

    std::cout << std::this_thread::get_id()
        << " finished computing..." << std::endl;
    return ret;
}

void do_print_ten(char c, int ms)
{
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        std::cout << c;
    }
}
void ThreadDemo::TestAsync2()
{
    // call function asynchronously:
    std::future<bool> fut = std::async(is_prime, 444444443);

    // do something while waiting for function to set future:
    std::cout << "checking, please wait";
    std::chrono::milliseconds span(100);
    //wait_until() 可以设置一个系统绝对时间点 abs_time
    while (fut.wait_for(span) == std::future_status::timeout)
        std::cout << '.';

    bool x = fut.get();         // retrieve return value

    std::cout << "\n444444443 " << (x ? "is" : "is not") << " prime.\n";

    //
    std::future<int> fut2 = std::async(do_get_value);
    //调用该函数之后，该 std::future 对象本身已经不和任何共享状态相关联，因此该 std::future 的状态不再是 valid 的了
    std::cout << "fut2 is valid: " << fut2.valid() << std::endl;
    std::shared_future<int> shared_fut = fut2.share();
    std::cout << "fut2 is valid: " << fut2.valid() << std::endl;
    // 共享的 future 对象可以被多次访问.
    std::cout << "value: " << shared_fut.get() << '\n';
    std::cout << "its double: " << shared_fut.get() * 2 << '\n';

    //
    std::promise<int> prom;
    std::future<int> fut3 = prom.get_future();

    std::thread th1(get_int, std::ref(prom));
    std::thread th2(print_int, std::ref(fut3));

    th1.join();
    th2.join();

    //
    // 由默认构造函数创建的 std::future 对象,
    // 初始化时该 std::future 对象处于为 invalid 状态.
    std::future<int> foo, bar;
    foo = std::async(do_get_value); // move 赋值, foo 变为 valid.
    bar = std::move(foo); // move 赋值, bar 变为 valid, 而 move 赋值以后 foo 变为 invalid.

    if (foo.valid())
        std::cout << "foo's value: " << foo.get() << '\n';
    else
        std::cout << "foo is not valid\n";

    if (bar.valid())
        std::cout << "bar's value: " << bar.get() << '\n';
    else
        std::cout << "bar is not valid\n";

    //wait的另一个用法，一直阻塞，知道ready
    // call function asynchronously:
    std::future <bool> fut4 = std::async(is_prime, 194232491);

    std::cout << "Checking...\n";
    fut4.wait();

    std::cout << "\n194232491 ";
    if (fut4.get()) // guaranteed to be ready (and not block) after wait returns
        std::cout << "is prime.\n";
    else
        std::cout << "is not prime.\n";

    //
    std::future<double> f(std::async(std::launch::async, ThreadTask, 100000000));

#if 0
    while (f.wait_until(std::chrono::system_clock::now() + std::chrono::seconds(1))
        != std::future_status::ready) {
        std::cout << "task is running...\n";
    }
#else
    while (f.wait_for(std::chrono::seconds(1))
        != std::future_status::ready) {
        std::cout << "task is running...\n";
    }
#endif

    std::cout << f.get() << std::endl;

    //
    std::cout << "with launch::async:\n";
    //Asynchronous: 异步任务会在另外一个线程中调用，并通过共享状态返回异步任务的结果（一般是调用 std::future::get() 获取异步任务的结果）。
    std::future <void> foo1 =
        std::async(std::launch::async, do_print_ten, '*', 100);
    std::future <void> bar1 =
        std::async(std::launch::async, do_print_ten, '@', 200);
    // async "get" (wait for foo and bar to be ready):
    foo1.get();
    bar1.get();
    std::cout << "\n\n";

    std::cout << "with launch::deferred:\n";
    //Deferred: 异步任务将会在共享状态被访问时调用，相当与按需调用（即延迟(deferred)调用）。
    foo1 = std::async(std::launch::deferred, do_print_ten, '*', 100);
    bar1 = std::async(std::launch::deferred, do_print_ten, '@', 200);
    // deferred "get" (perform the actual calls):
    foo1.get();
    bar1.get();
    std::cout << '\n';
}

//https://blog.csdn.net/weixin_34413065/article/details/86403189?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-4.control&dist_request_id=&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-4.control
std::mutex mtx1;           // mutex for critical section

//这个使用mutex的方法以前没有使用过
void print_thread_id(int id) {
    mtx1.lock();
    std::lock_guard<std::mutex> lck(mtx1, std::adopt_lock);
    std::cout << "thread #" << id << '\n';
}

void print_even(int x) {
    if (x % 2 == 0) std::cout << x << " is even\n";
    else throw (std::logic_error("not even"));
}

//对于我来说这个是经常使用的常规方法
void print_thread_id2(int id) {
    try {
        // using a local lock_guard to lock mtx guarantees unlocking on destruction / exception:
        std::lock_guard<std::mutex> lck(mtx1);
        print_even(id);
    }
    catch (std::logic_error&) {
        std::cout << "[exception caught]\n";
    }
}

std::mutex foo1, bar1;

void task_a() {
    std::lock(foo1, bar1);         // simultaneous lock (prevents deadlock)
    std::unique_lock<std::mutex> lck1(foo1, std::adopt_lock);
    std::unique_lock<std::mutex> lck2(bar1, std::adopt_lock);
    std::cout << "task a\n";
    // (unlocked automatically on destruction of lck1 and lck2)
}

void task_b() {
    // foo.lock(); bar.lock(); // replaced by:
    std::unique_lock<std::mutex> lck1, lck2;
    lck1 = std::unique_lock<std::mutex>(bar1, std::defer_lock);
    lck2 = std::unique_lock<std::mutex>(foo1, std::defer_lock);
    std::lock(lck1, lck2);       // simultaneous lock (prevents deadlock)
    std::cout << "task b\n";
    // (unlocked automatically on destruction of lck1 and lck2)
}

std::mutex mtx3;           // mutex for critical section

void print_fifty(char c) {
    std::unique_lock<std::mutex> lck;         // default-constructed
    lck = std::unique_lock<std::mutex>(mtx3);  // move-assigned
    for (int i = 0; i < 50; ++i) { std::cout << c; }
    std::cout << '\n';
}

void ThreadDemo::TestLock()
{
    //
    std::thread threads1[10];
    // spawn 10 threads:
    for (int i = 0; i < 10; ++i)
        threads1[i] = std::thread(print_thread_id, i + 1);

    for (auto& th : threads1) th.join();

    //
    std::thread threads2[10];
    // spawn 10 threads:
    for (int i = 0; i < 10; ++i)
        threads2[i] = std::thread(print_thread_id2, i + 1);

    for (auto& th : threads2) th.join();

    //
    std::thread th1(task_a);
    std::thread th2(task_b);

    th1.join();
    th2.join();

    //
    std::thread th3(print_fifty, '*');
    std::thread th4(print_fifty, '$');

    th3.join();
    th4.join();
}

//https://blog.csdn.net/fengbingchun/article/details/104127352
///
// reference: http://www.cplusplus.com/reference/future/packaged_task/
int test_packaged_task_1()
{

    { // constructor/get_future/operator=/valid
        std::packaged_task<int(int)> foo; // default-constructed
        std::packaged_task<int(int)> bar([](int x) { return x * 2; }); // initialized

        foo = std::move(bar); // move-assignment
        std::cout << "valid: " << foo.valid() << "\n";
        std::future<int> ret = foo.get_future(); // get future
        std::thread(std::move(foo), 10).detach(); // spawn thread and call task

        int value = ret.get(); // wait for the task to finish and get result
        std::cout << "The double of 10 is " << value << ".\n";
    }

    { // reset/operator()
        std::packaged_task<int(int)> tsk([](int x) { return x * 3; }); // package task

        std::future<int> fut = tsk.get_future();
        tsk(33);
        std::cout << "The triple of 33 is " << fut.get() << ".\n";

        // re-use same task object:
        tsk.reset();
        fut = tsk.get_future();
        std::thread(std::move(tsk), 99).detach();
        std::cout << "Thre triple of 99 is " << fut.get() << ".\n";
    }

    { // constructor/get_future
        auto countdown = [](int from, int to) {
            for (int i = from; i != to; --i) {
                std::cout << i << '\n';
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            std::cout << "Lift off!\n";
            return from - to;
        };

        std::packaged_task<int(int, int)> tsk(countdown); // set up packaged_task
        std::future<int> ret = tsk.get_future(); // get future

        std::thread th(std::move(tsk), 5, 0); // spawn thread to count down from 5 to 0

        int value = ret.get(); // wait for the task to finish and get result
        std::cout << "The countdown lasted for " << value << " seconds.\n";

        th.join();
    }

    return 0;
}

///
// reference: https://en.cppreference.com/w/cpp/thread/packaged_task
int test_packaged_task_2()
{
    { // lambda
        std::packaged_task<int(int, int)> task([](int a, int b) { return std::pow(a, b); });
        std::future<int> result = task.get_future();

        task(2, 9);
        std::cout << "task_lambda:\t" << result.get() << '\n';
    }

    { // bind
        std::packaged_task<int()> task(std::bind([](int x, int y) { return std::pow(x, y); }, 2, 11));
        std::future<int> result = task.get_future();

        task();
        std::cout << "task_bind:\t" << result.get() << '\n';
    }

    { // thread
        std::packaged_task<int(int, int)> task([](int x, int y) { return std::pow(x, y); });
        std::future<int> result = task.get_future();

        std::thread task_td(std::move(task), 2, 10);
        task_td.join();
        std::cout << "task_thread:\t" << result.get() << '\n';
    }

    return 0;
}

///
// reference: https://thispointer.com/c11-multithreading-part-10-packaged_task-example-and-tutorial/
struct DBDataFetcher {
    std::string operator()(std::string token)
    {
        // Do some stuff to fetch the data
        std::string data = "Data From " + token;
        return data;
    }
};

int test_packaged_task_3()
{
    // Create a packaged_task<> that encapsulated a Function Object
    std::packaged_task<std::string(std::string)> task(std::move(DBDataFetcher()));

    // Fetch the associated future<> from packaged_task<>
    std::future<std::string> result = task.get_future();

    // Pass the packaged_task to thread to run asynchronously
    std::thread th(std::move(task), "Arg");

    // Join the thread. Its blocking and returns when thread is finished.
    th.join();

    // Fetch the result of packaged_task<> i.e. value returned by getDataFromDB()
    std::string data = result.get();
    std::cout << data << std::endl;

    return 0;
}

///
// reference: https://stackoverflow.com/questions/18143661/what-is-the-difference-between-packaged-task-and-async
int test_packaged_task_4()
{
    // sleeps for one second and returns 1
    auto sleep = []() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 1;
    };

    { // std::packaged_task
        // >>>>> A packaged_task won't start on it's own, you have to invoke it
        std::packaged_task<int()> task(sleep);

        auto f = task.get_future();
        task(); // invoke the function

        // You have to wait until task returns. Since task calls sleep
        // you will have to wait at least 1 second.
        std::cout << "You can see this after 1 second\n";

        // However, f.get() will be available, since task has already finished.
        std::cout << f.get() << std::endl;
    }

    { // std::async
        // >>>>> On the other hand, std::async with launch::async will try to run the task in a different thread :
        auto f = std::async(std::launch::async, sleep);
        std::cout << "You can see this immediately!\n";

        // However, the value of the future will be available after sleep has finished
        // so f.get() can block up to 1 second.
        std::cout << f.get() << "This will be shown after a second!\n";
    }

    return 0;
}


void ThreadDemo::TestPackagedTask2()
{
    test_packaged_task_1();
    test_packaged_task_2();
    test_packaged_task_3();
    test_packaged_task_4();
}









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




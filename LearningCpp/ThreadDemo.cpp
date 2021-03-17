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
    std::packaged_task<int(int, int)> task(countdown); // ���� packaged_task
    std::future<int> ret = task.get_future(); // ����� packaged_task ����״̬������� future ����.
    std::thread th(std::move(task), 10, 0);   //����һ�����߳���ɼ�������.
    int value = ret.get();                    // �ȴ�������ɲ���ȡ���.
    std::cout << "The countdown lasted for " << value << " seconds.\n";
    th.join();
}

void ThreadDemo::TestPackageTaskConstructor()
{
    std::packaged_task<int(int)> foo; // Ĭ�Ϲ��캯��.

   // ʹ�� lambda ���ʽ��ʼ��һ�� packaged_task ����.
    std::packaged_task<int(int)> bar([](int x) {return x * 2; });

    foo = std::move(bar); // move-��ֵ������Ҳ�� C++11 �е�������.

    // ��ȡ�� packaged_task ����״̬������� future ����.
    std::future<int> ret = foo.get_future();

    std::thread(std::move(foo), 10).detach(); // �����̣߳����ñ���װ������.

    int value = ret.get(); // �ȴ�������ɲ���ȡ���.
    std::cout << "The double of 10 is " << value << ".\n";
}

// �����߳�������һ�� int(int) packaged_task.
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
    std::future<int> fut = tsk.get_future();   // ��ȡ future ����.
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "thread" << std::endl;
    std::thread(std::move(tsk), 100).detach();   // �������̲߳�����packaged_task.
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "get" << std::endl;
    int value = fut.get();                     // �ȴ��������, ����ȡ���.
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
    //����׳�
    long long ret = 1;
    for (int i = 1; i <= n; i++)
        ret *= i;
    return ret;

}

void ThreadDemo::TestAsync()
{
    //����1
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

    //����2
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
std::mutex mtx; // ȫ���໥�ų���.
std::condition_variable cv; // ȫ����������.
bool ready = false; // ȫ�ֱ�־λ
void do_print_id(int id)
{
    std::unique_lock <std::mutex> lck(mtx);
    while (!ready) // �����־λ��Ϊ true, ��ȴ�...
        cv.wait(lck); // ��ǰ�̱߳�����, ��ȫ�ֱ�־λ��Ϊ true ֮��,
    // �̱߳�����, �����������д�ӡ�̱߳��id.
    std::cout << "thread " << id << '\n';
}

void go()
{
    std::unique_lock <std::mutex> lck(mtx);
    ready = true; // ����ȫ�ֱ�־λΪ true.
    cv.notify_all(); // ����ȫ���߳�.
}

//��һ������
int cargo = 0;
bool shipment_available()
{
    return cargo != 0;
}

// �������߳�.
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
    //ʾ��1
    std::thread threads[10];
    // spawn 10 threads:
    for (int i = 0; i < 10; ++i)
        threads[i] = std::thread(do_print_id, i);

    std::cout << "10 threads ready to race...\n";
    go(); // go!

    for (auto& th : threads)
        th.join();

    //ʾ��2
    std::thread consumer_thread(consume, 10); // �������߳�.

    // ���߳�Ϊ�������߳�, ���� 10 ����Ʒ.
    for (int i = 0; i < 10; ++i) {
        while (shipment_available())
            std::this_thread::yield();
        std::unique_lock <std::mutex> lck(mtx);
        cargo = i + 1;
        cv.notify_one();
    }

    consumer_thread.join();
}

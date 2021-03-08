#include "ThreadDemo.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <future>

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

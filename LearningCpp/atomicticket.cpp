#include "testlang.h"
#include <iostream>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <semaphore>
using namespace std::literals;  // for duration literals

void testAtomicTicket()
{
    std::queue<char> values;  // queue of values
    std::mutex valuesMx;      // mutex to protect access to the queue

    // initialize a queue with some values
    // - no mutex because no other thread is running yet:
    for (int i = 0; i < 1000; ++i) {
        values.push(char(i % (128 - 32) + 32));
    }

    // limit the availability of threads with a ticket system:
    std::atomic<int> maxTicket{ 0 };  // maximum requested ticket no
    std::atomic<int> actTicket{ 0 };  // current allowed ticket no

    // create and start a pool of numThreads threads:
    constexpr int numThreads = 10;
    std::vector<std::jthread> pool;
    for (int idx = 0; idx < numThreads; ++idx) {
        pool.push_back(std::jthread{ [&, idx](std::stop_token st) {
                                      while (!st.stop_requested()) {
                                          // get next value from the queue:
                                          char val;
                                          {
                                            std::lock_guard lg{valuesMx};
                                            val = values.front();
                                            values.pop();
                                          }

                                          // request a ticket and wait until it is called:
                                          int myTicket{++maxTicket};
                                          int act = actTicket.load();
                                          while (act < myTicket) {
                                            actTicket.wait(act);
                                            act = actTicket.load();
                                          }

                                          // print the value 10 times:
                                          for (int i = 0; i < 10; ++i) {
                                            std::cout.put(val).flush();
                                            auto dur = 20ms * ((idx % 3) + 1);
                                            std::this_thread::sleep_for(dur);
                                          }

                                          // next ticket please:
                                          ++actTicket;
                                          actTicket.notify_all();
                                        }
                                      } });
    }

    // enable and disable threads in the thread pool:
    auto enable = [&, oldNum = 0](int newNum) mutable {
        actTicket += newNum - oldNum;            // enable/disable tickets
        if (newNum > 0) actTicket.notify_all();  // wake up waiting threads
        oldNum = newNum;
    };

    for (int num : {0, 3, 5, 2, 0, 1}) {
        std::cout << "\n\n====== enable " << num << " threads" << std::endl;
        enable(num);
        std::this_thread::sleep_for(2s);
    }

    std::quick_exit(0);
}

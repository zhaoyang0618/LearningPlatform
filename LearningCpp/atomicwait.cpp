#include "testlang.h"
#include <iostream>
#include <thread>
#include <atomic>
using namespace std::literals;

void testAtomicWait()
{
    std::atomic<int> aVal{ 0 };

    // reader:
    std::jthread tRead{ [&] {
                         int lastX = aVal.load();
                         while (lastX >= 0) {
                           aVal.wait(lastX);
                           std::cout << "=> x changed to " << lastX << std::endl;
                           lastX = aVal.load();
                         }
                         std::cout << "READER DONE: " << lastX << std::endl;
                      } };

    // writer:
    std::jthread tWrite{ [&] {
                          for (int newVal : { 17, 34, 3, 42, -1}) {
                            std::this_thread::sleep_for(5000ns);
                            std::cout << "write: " << newVal << std::endl;
                            aVal = newVal;
                            aVal.notify_all();
                          }
                       } };
    //...
}

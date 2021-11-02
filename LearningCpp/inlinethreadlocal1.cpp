#include "inlinethreadlocal.h"
#include <thread>
void foo2();
void testThreadLocal()
{
	myThreadData.print("testThreadLocal() begin:");
	myThreadData.gName = "thread1 name";
	myThreadData.tName = "thread1 name";
	myThreadData.lName = "thread1 name";
	myThreadData.print("testThreadLocal() later:");
	std::thread t(foo2);
	t.join();
	myThreadData.print("testThreadLocal() end:");
}

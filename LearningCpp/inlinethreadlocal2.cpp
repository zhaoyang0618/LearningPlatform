#include "inlinethreadlocal.h"
void foo2()
{
	myThreadData.print("foo() begin:");
	myThreadData.gName = "thread2 name";
	myThreadData.tName = "thread2 name";
	myThreadData.lName = "thread2 name";
	myThreadData.print("foo() end:");
}

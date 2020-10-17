#pragma once
#include <type_traits>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <map>


void testSTLCookbook();

//下面这个例子的关键是inline用于变量
class process_monitor {
public:
	static const inline std::string standard_string
	{ "some static globally available string" };
};
inline process_monitor global_process_monitor;

///
/// C++17 变长的using声明
/// 
#pragma once

/// 继承所有基类里的函数调用运算符
template<typename... Ts>
struct overload : Ts ...
{
	using Ts::operator()...;
};

template<typename ... Ts>
overload(Ts...)->overload<Ts...>;

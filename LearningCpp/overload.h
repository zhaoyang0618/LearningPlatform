///
/// C++17 �䳤��using����
/// 
#pragma once

/// �̳����л�����ĺ������������
template<typename... Ts>
struct overload : Ts ...
{
	using Ts::operator()...;
};

template<typename ... Ts>
overload(Ts...)->overload<Ts...>;

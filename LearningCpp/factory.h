#pragma once
#include <utility>
template <typename T, typename... Args>
T create(Args&&... args)
{
	...
		return T{ std::forward<Args>(args)... };
}

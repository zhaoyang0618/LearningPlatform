#pragma once
#include <type_traits>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <unordered_map>
#include <stdexcept>
#include <stack>
#include <cmath>
#include <sstream>
#include <algorithm>

/// 在不需要保证列表顺序的情况下，删除操作有一个更快的方法
/// 直接把最后一个替换到当前位置
/// 然后删除最后一个:因为这个的删除是很快的
template <typename T>
void quick_remove_at(std::vector<T>& v,
	typename std::vector<T>::iterator it)
{
	if (it != std::end(v)) {
		*it = std::move(v.back());
		v.pop_back();
	}
}

template <typename T>
void quick_remove_at(std::vector<T>& v, std::size_t idx)
{
	if (idx < v.size()) {
		v[idx] = std::move(v.back());
		v.pop_back();
	}
}

/// 要求原来的容器就是经过排序的插入保证顺序
/// sort(begin(v), end(v));
template <typename C, typename T>
void insert_sorted(C& v, const T& item)
{
	const auto insert_pos(lower_bound(begin(v), end(v), item));
	v.insert(insert_pos, item);
}

void testSTLContainers();

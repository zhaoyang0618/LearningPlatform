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

/// �ڲ���Ҫ��֤�б�˳�������£�ɾ��������һ������ķ���
/// ֱ�Ӱ����һ���滻����ǰλ��
/// Ȼ��ɾ�����һ��:��Ϊ�����ɾ���Ǻܿ��
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

/// Ҫ��ԭ�����������Ǿ�������Ĳ��뱣֤˳��
/// sort(begin(v), end(v));
template <typename C, typename T>
void insert_sorted(C& v, const T& item)
{
	const auto insert_pos(lower_bound(begin(v), end(v), item));
	v.insert(insert_pos, item);
}

void testSTLContainers();

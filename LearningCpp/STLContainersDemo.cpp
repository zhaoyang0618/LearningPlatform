#include "STLContainersDemo.h"

template<typename T>
void print(const std::vector<T>& v)
{
	std::cout << "size: " << v.size() << " capacity: " << v.capacity() << std::endl;
	for (auto i : v) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;
}

void testSTLContainers()
{
	std::cout << "����STL Conatiners" << std::endl;
	//Using the erase-remove idiom on std::vector
	std::vector<int> v{ 1, 2, 3, 2, 5, 2, 6, 2, 4, 8 };
	print(v);
	//ɾ���б��е�Ԫ��2
	const auto new_end (remove(begin(v), end(v), 2));
	print(v);
	//Interestingly, that was only one of the two steps. The vector still has the same size.The next line makes it actually shorter.
	v.erase(new_end, end(v));
	print(v);

	//
	const auto odd([](int i) { return i % 2 != 0; });
	v.erase(std::remove_if(std::begin(v), std::end(v), odd), std::end(v));
	print(v);
	//����Ĳ����������ڴ�ķ��䣬�Լ����ݵ��ƶ�
	v.shrink_to_fit();
	print(v);
}

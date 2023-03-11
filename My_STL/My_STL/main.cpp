#include "Y_memory.h"
#include "Y_vector.h"
#include <vector>
#include <string>
USING_YLH
void testAlloc()
{
	alloc all;
#if 1
	int* b = (int*)all.allocate(1);
	*b = 10;
	std::cout << *b << '\n';
	int* c = (int*)all.allocate(1);
	*c = 20;
	all.deallocate(c, 1);
	delete b;
#else
	int* b = (int*)all.allocate(130);//调用第一级
	*b = 10;
	delete b;
#endif
}


template <typename InputIterator>
void show(InputIterator first, InputIterator last) {
	for (; first != last; ++first) {
		std::cout << *first << ' ';
	}
	std::cout << '\n';
}

//push_back与insert测试
void testVector()
{
#if 1
	vector<int> vec;
	auto vector_dbg = [&]() {
		std::cout << "vec的元素个数:" << vec.size() << '\n';
		std::cout << "vec的尾元素:" << vec.back() << '\n';
		std::cout << "vec的头元素:" << vec.front() << '\n';
		std::cout << "vec的容量大小:" << vec.capacity() << '\n';
		std::cout << "vec是否为空? " << vec.empty() << '\n';
	};
	show(vec.begin(), vec.end());
	//vec.erase(vec.begin(), vec.begin() + 2);
	for (int i = 1; i <= 10; i++) {

		vec.push_back(i);
	}
	show(vec.begin(), vec.end());
	vector_dbg();
	
	vec.insert(vec.get(2), 10, 5);
	show(vec.begin(), vec.end());
	vec.insert(vec.get(1), 9, 1);

	show(vec.begin(), vec.end());
	vec.insert(vec.get(9), 1, 9999);

	vec.insert(vec.end(), 1, 6666);
	vec.insert(vec.begin(), 999);
	vector_dbg();

	vec.pop_back();
	vector_dbg();
	vec.pop_back();
	vector_dbg();
	vec.pop_back();
	vector_dbg();
#endif
}

void testPush_back() {
	vector<int> vec{ 1,2,3,4,5,6 };
	show(vec.begin(), vec.end());
}

int main()
{
	testVector();
	//testPush_back();

	return 0;
}
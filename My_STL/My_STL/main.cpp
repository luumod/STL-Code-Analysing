#include "Y_memory.h"
#include "Y_vector.h"
#include <vector>
#include <list>
#include "Y_list.h"
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

//list test
void testList() {
	list<int> ls;
	for (int i = 1; i <= 5; i++) {
		ls.push_back(i);
		show(ls.begin(), ls.end());
	}
	for (int i = 5; i <= 10; i++) {
		ls.push_front(i);
		show(ls.begin(), ls.end());
	}
#if 0
	while (!ls.empty()) {
#if 0
		ls.pop_front();
#else
		ls.pop_back();
#endif
		show(ls.begin(), ls.end());
	}
#endif
	/*for (int i = 1; i <= 5; i++) {
		ls.remove(i);
		show(ls.begin(), ls.end());
	}
	*/
	list<int> temp{ 1,2,3,4 };
	list<int>::iterator cur(ls.begin());
	for (int i = 1; i <= 4; i++) {
		++cur;
	}
	std::cout << *cur << std::endl;
	ls.splice(cur, temp);
	show(ls.begin(), ls.end());
	ls.sort();
		show(ls.begin(), ls.end());
	//show(temp.begin(), temp.end());
	/*show(ls.begin(), ls.end());
	std::cout << "Size: " << ls.size() << std::endl;
	std::cout << "------------------------------------\n";
	list<int> ls2{ 0,1,1,1,1,1,3};
	show(ls2.begin(), ls2.end());
	ls2.unique();
	show(ls2.begin(), ls2.end());
	


	std::cout << "Size: " << ls2.size() << std::endl;*/
}

int main()
{
	//testVector();
	//testPush_back();
	testList();
	list<int> ls{ 1,2,13,14,0 };
	list<int> ls2{ 5,6,7,8};
	ls.merge(ls2);
	show(ls.begin(), ls.end());
	show(ls2.begin(), ls2.end());

	ls.reverse();
	show(ls.begin(), ls.end());
	ls.sort();
	ls.show();
	return 0;
}
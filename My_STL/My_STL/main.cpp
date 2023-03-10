#include <vld.h>
#include "Y_memory.h"
#include <vector>
#include <string>
USING_YLH

class Foo
{
public:
	int arr;
	int* pointer;
	char* num;
	Foo() :arr(10), pointer(new int{ 20 }), num(new char{ 'a' }) {}
	~Foo()
	{
		if (pointer)
		{
			delete pointer;
			pointer = nullptr;
		}
		if (num)
		{
			delete num;
			num = nullptr;
		}
	}
};

struct uni
{
	uni* next;
};

void test1()
{
	std::vector<int> vec{ 1,2,3,5,6,9,78,7,45,5,32,1,3,3,1,23 }, vec2(vec.size());
	int num = 10;
	//ylh::uninitialized_fill(vec.begin(), vec.end(), num);
	//ylh::uninitialized_copy(vec.begin(), vec.end(), vec2.begin());
	//ylh::uninitialized_fill_n(vec2.begin(), 5, 10);

}
int main()
{
	alloc all;
	int* b = (int*)all.allocate(1);
	*b = 10;
	all.deallocate(b, 1);
	delete b;

	return 0;
}
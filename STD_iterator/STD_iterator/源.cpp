#include <iostream>

template <typename T,typename Category,typename Distance=ptrdiff_t,
	typename Reference=T&,typename Pointer=T*>
struct iterator
{
	using value_type = T;
	using difference_type = Distance;
	using reference = Reference;
	using pointer = Pointer;
	using iterator_category = Category;
};

template <typename T>
class ListIter:public iterator<T,std::forward_iterator_tag>
{
	//类内
};




int main()
{
	
	return 0;
}
#include <iostream>
#include <vector>
#include "mylist-iter.h"

template <typename T>
struct Mystu
{
	typedef T difference_type;//内嵌型别
	T* ptr;
	Mystu(T* ptr = NULL) :ptr(ptr) {}
	T& operator*()const { return *ptr; }
};

//原版
template <typename T>
struct iterator_traits
{
	typedef typename T::difference_type difference_type;
};
//针对原生指针的偏特化版本
template <typename T>
struct iterator_traits<T*>
{
	typedef T difference_type;
};

//针对原生指针const的偏特化版本
template <typename T>
struct iterator_traits<const T*>
{
	typedef T difference_type;
};

template <typename Iter, typename T>
typename iterator_traits<Iter>::difference_type _count(Iter beg, Iter end, const T& val)
{
	// Iter=ListIter  ->  difference_type=ListIter::difference_type=ptrsdiff_t 
	typename iterator_traits<Iter>::difference_type n = 0;//ptrdiff_t
	for (; beg != end; ++beg)
	{
		if (*beg == val)
			++n;
	}
	return n;
}

int main()
{
	List<int> p;
	for (int i = 1; i <= 10; i++)
	{
		p.insert_end(i);
	}
	p.insert_end(5);
	p.show();

	ListIter<ListNode<int>> beg(p.front());
	ListIter<ListNode<int>> end;
	ListIter<ListNode<int>> Iter;

	auto ppp =  _count(beg, end, 5);
	std::cout << ppp << std::endl;
	return 0;
}
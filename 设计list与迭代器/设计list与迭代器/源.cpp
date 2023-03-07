#include <iostream>
#include <algorithm>
#include "mylist-iter.h"

template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
	/*
	传入后first和last调用ListIter类的重载的!=运算符。
	但是*first得到的是ListNode<T>类型的值，因此为了比较与T类型的值还需要进行操作：
		1. 直接在外部添加针对ListNode<T>和T的!=的重载运算符
		2. 在ListNode<T>内添加重载!=的运算符
	*/
	while (first != last && *first != value) ++first;

	return first;
}

#if 0
template <typename T>
bool operator!=(const ListNode<T>& pnode, const T& val)
{
	return pnode.value() != val;
}
#endif

int main()
{
	List<int> mlist;
	for (int i = 1; i <= 5; i++)
	{
		mlist.insert_front(i);
		mlist.insert_end(i + 10);
	}
	mlist.show();

	ListIter<ListNode<int>> beg(mlist.front());
	ListIter<ListNode<int>> end;
	ListIter<ListNode<int>> Iter;
	Iter = find(beg, end, 11);
	if (Iter == end) std::cout << "没有找到!" << '\n';
	else std::cout << "找到了! " << Iter->value() << '\n';
	return 0;
}

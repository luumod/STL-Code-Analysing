#ifndef MY_LIST_ITER
#define MY_LIST_ITER
#include "mylist.h"
template <typename Node>
class ListIter
{
public:
	ListIter<Node>(Node* p = NULL)
		:_ptr(p) {}
	Node& operator*()const { return *_ptr; }
	Node* operator->()const { return _ptr; }
	ListIter& operator++()
	{
		//前置++
		_ptr = _ptr->next(); //! 暴露了ListNode的next函数
		return *this;
	}
	ListIter operator++(int)
	{
		//后置++
		ListIter temp = *this;
		++* this;
		return temp;
	}
	bool operator==(const ListIter& lhs)const { return _ptr == lhs._ptr; }
	bool operator!=(const ListIter& lhs)const { return _ptr != lhs._ptr; }
private:
	Node* _ptr;
};

#endif
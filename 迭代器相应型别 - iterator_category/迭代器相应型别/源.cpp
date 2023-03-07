#include <iostream>
#include <vector>

#if 0
//对于只读迭代器：input 可读可写迭代器：forward 都是这样的
template <typename inputiterator,typename distance>
void advance(inputiterator iter, distance n)
{
	while (n--) ++iter;
}
//对于双向迭代器：bidirectional 
template <typename bidirectional, typename distance>
void advance(bidirectional iter, distance n)
{
	if (n >= 0)
	{
		while (n--) ++iter;
	}
	else
	{
		while (n++) --iter;
	}
}
//对于随机迭代器：random
template <typename randomiterator, typename distance>
void advance(randomiterator iter, distance n)
{
	iter += n;//随机迭代器支持任意的指针运算
}
#endif

//-------------------------------------------------------
// 
//五个标记类型
struct Input_Iterator_tag {};
struct Output_Iterator_tag {};
struct Forward_Iterator_tag: public Input_Iterator_tag {};
struct Bidirectional_Iterator_tag: public Forward_Iterator_tag {};
struct Random_Iterator_tag:public Bidirectional_Iterator_tag {};


//对于只读迭代器：Input 可读可写迭代器：Forward 都是这样的
template <typename InputIterator, typename Distance>
void _advance(InputIterator Iter, Distance n,Input_Iterator_tag)
{
	while (n--) ++Iter;
}
//对于可读可写迭代器:直接调用Input
template <typename InputIterator, typename Distance>
void _advance(InputIterator Iter, Distance n, Forward_Iterator_tag)
{
	_advance(Iter, n, Input_Iterator_tag());
}
//对于双向迭代器：Bidirectional 
template <typename Bidirectional, typename Distance>
void _advance(Bidirectional Iter, Distance n,Bidirectional_Iterator_tag)
{
	if (n >= 0)
	{
		while (n--) ++Iter;
	}
	else
	{
		while (n++) --Iter;
	}
}
//对于随机迭代器：Random
template <typename RandomIterator, typename Distance>
void _advance(RandomIterator Iter, Distance n,Random_Iterator_tag)
{
	Iter += n;//随机迭代器支持任意的指针运算
}

template <typename T>
struct iterator_traits
{
	typedef typename T::iterator_category iterator_category;
};
//由于Random_Iterator可以进行任意的指针操作，所以任何指针都是Random_Iterator_tag的版本
//原指针的偏特化
template <typename T>
struct iterator_traits<T*>
{
	//指针类型是Random_Iterator_tag的版本
	typedef Random_Iterator_tag iterator_category;
};
//const指针的偏特化
template <typename T>
struct iterator_traits<const T*>
{
	//const指针类型是Random_Iterator_tag的版本
	typedef Random_Iterator_tag iterator_category;
};

//iterator_category函数!!!!!!!!!!!!!!!!!!!!!!!
template <typename Iter>
inline typename iterator_traits<Iter>::iterator_category iterator_category(const Iter&)
{
	//推断Iter的类型，确定迭代器标签类型
	typedef typename iterator_traits<Iter>::iterator_category category;
	return category();//创建匿名标签对象，用于确定第三个参数选择哪一个版本
}

//执行函数
template <typename InputIterator,typename Distance>
void m_advance(InputIterator& it, Distance n)
{
	_advance(it, n, iterator_category(it));//返回一个标签，用于确定匹配那一个版本
}


//--------------------------------------------------------
//_distance函数
//--------------------------------------------------------

template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type  
	_distance(InputIterator beg, InputIterator end, Input_Iterator_tag)
{
	typename iterator_traits<InputIterator>::difference_type n = 0;
	while (beg != end)
	{
		++beg;
		++n;
	}
	return n;
}

template <typename RandomIterator>
typename iterator_traits<RandomIterator>::difference_type
	_distance(RandomIterator beg, RandomIterator end, Random_Iterator_tag)
{
	//直接计算差距
	return end - beg;
}

template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
	distance(InputIterator beg, InputIterator end)
{
	typedef typename iterator_traits<InputIterator>::iterator_category category;
	_distance(beg, end, category());
}


signed main()
{
	
	return 0;
}
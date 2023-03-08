#ifndef MY_ITERATOR_H
#define MY_ITERATOR_H
#include <iostream>
/*
1. 五种迭代器类型
2. iterator类
3. 迭代器萃取类，针对指针的偏特化
4. 决定某个迭代器的型别（category，value_type，difference_type）
5. distance函数，advance函数
*/

//1. 五种迭代器类型
struct InputIterator_tag {};//读入迭代器
struct OutputIterator_tag {};//输出迭代器
struct ForwardIterator_tag {};//可读可写迭代器
struct Bidirectional_Iterator_tag {};//双向迭代器
struct RandomAccessIterator_tag {};//随机迭代器

//2. iterator类
template <typename category, typename T, typename Distance = std::ptrdiff_t,
	typename Pointer = T*, typename Reference = T&>
struct iterator
{
	using value_type = T;
	using difference_type = Distance;
	using pointer = Pointer;
	using reference = Reference;
	using iterator_category = category;
};

//3. 迭代器型别萃取类
template <typename Iter>
struct iterator_traits
{
	using value_type = typename Iter::value_type;
	using difference_type = typename Iter::difference_type;
	using pointer = typename Iter::pointer;
	using reference = typename Iter:reference;
	using iterator_category = typename Iter::iterator_category;
};

//针对指针的偏特化
template <typename T>
struct iterator_traits<T*>
{
	using value_type = T;
	using difference_type = ptrdiff_t;
	using pointer = T*;
	using reference = T&;
	using iterator_category = RandomAccessIterator_tag;
};
template <typename T>
struct iterator_traits<const T*>
{
	using value_type = T;
	using difference_type = ptrdiff_t;
	using pointer = const T*;
	using reference = const T&;
	using iterator_category = RandomAccessIterator_tag;
};

//4. 决定某个迭代器的类型（category，value_type，difference_type）
//4.1 category
template <typename Iter>
inline typename iterator_traits<Iter>::iterator_category
	iterator_category(const Iter& it)
{
	using category = typename iterator_traits<Iter>::iterator_category;
	return category();
}
//决定某个迭代器的difference_type
template <typename Iter>
inline typename iterator_traits<Iter>::difference_type*
	difference_type(const Iter& it)
{
	return static_cast<typename iterator_traits<Iter>::difference_type*>(0);
}
//决定某个迭代器的value_type
template <typename Iter>
inline typename iterator_traits<Iter>::value_type*
	value_type(const Iter& it)
{
	return static_cast<typename iterator_traits<Iter>::value_type*>(0);
}

//5. distance函数，advance函数
template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
	_distance(InputIterator begin, InputIterator end,InputIterator_tag)
{
	typename iterator_traits<InputIterator>::difference_type n = 0;
	while (begin != end)
	{
		begin++;
		n++;
	}
	return n;
}
template <typename RandomIterator>
typename iterator_traits<RandomIterator>::difference_type
	_distance(RandomIterator begin,RandomIterator end,RandomAccessIterator_tag)
{
	return end - begin;
}
//distance主函数
template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
	distance(InputIterator begin, InputIterator end)
{
	//using category = typename iterator_traits<InputIterator>::iterator_category;
	return _distance(begin, end, iterator_category());
}
//-----------------------------------------
//advance函数
template <typename InputIterator,typename Distance>
void _advance(InputIterator it,Distance n,InputIterator_tag)
{
	while (n--) ++it;
}
template <typename BidirectionalIterator,typename Distance>
void _advance(BidirectionalIterator it,Distance n,Bidirectional_Iterator_tag)
{
	if (n >= 0)
	{
		while (n--) ++it;
	}
	else
	{
		while (n++) --it;
	}
}
template <typename RandomIterator,typename Distance>
void _advance(RandomIterator it, Distance n,RandomAccessIterator_tag)
{
	it += n;
}
template <typename InputIterator,typename Distance>
void advance(InputIterator it, Distance n)
{
	_advance(it, n, iterator_category(it));
}

#endif // !MY_ITERATOR_H

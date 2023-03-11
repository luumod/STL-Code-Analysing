#ifndef Y_ITERATOR_H_
#define Y_ITERATOR_H_
#include "Y_config.h"

/*
迭代器存取
*/

//五种迭代器标签
struct InputIterator_tag {};
struct OutputIterator_tag {};
struct ForwardIterator_tag :public InputIterator_tag {};
struct Bibirectional_Iterator_tag :public ForwardIterator_tag {};
struct Random_Access_Iterator_tag :public Bibirectional_Iterator_tag {};

//iterator类
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

//迭代器萃取机
template <typename T>
struct iterator_traits
{
	using value_type = typename T::value_type;
	using difference_type = typename T::difference_type;
	using pointer = typename T::pointer;
	using reference = typename T::reference;
	using iterator_category = typename T::iterator_category;
};

template <typename T>
struct iterator_traits<T*>
{
	using value_type = T;
	using difference_type = ptrdiff_t;
	using pointer = T*;
	using reference = T&;
	using iterator_category = Random_Access_Iterator_tag;
};

template <typename T>
struct iterator_traits<const T*>
{
	using value_type = T;
	using difference_type = ptrdiff_t;
	using pointer = const T*;
	using reference = const T&;
	using iterator_category = Random_Access_Iterator_tag;
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
_distance(InputIterator begin, InputIterator end, InputIterator_tag)
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
_distance(RandomIterator begin, RandomIterator end, Random_Access_Iterator_tag)
{
	return end - begin;
}
//distance主函数
template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator begin, InputIterator end)
{
	//using category = typename iterator_traits<InputIterator>::iterator_category;
	return _distance(begin, end, iterator_category(begin));
}
//-----------------------------------------
//advance函数
template <typename InputIterator, typename Distance>
void _advance(InputIterator it, Distance n, InputIterator_tag)
{
	while (n--) ++it;
}
template <typename BidirectionalIterator, typename Distance>
void _advance(BidirectionalIterator it, Distance n, Bibirectional_Iterator_tag)
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
template <typename RandomIterator, typename Distance>
void _advance(RandomIterator it, Distance n, Random_Access_Iterator_tag)
{
	it += n;
}
template <typename InputIterator, typename Distance>
void advance(InputIterator it, Distance n)
{
	_advance(it, n, iterator_category(it));
}


#endif // !Y_ITERATOR_H_

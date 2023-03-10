#ifndef Y_UNINITIALIZED_H_
#define Y_UNINITIALIZED_H_
#include "Y_config.h"
#include "Y_type_traits.h"
#include "Y_construct.h"

/*
std::uninitialized_copy();//有返回值
std::uninitialized_fill();//无返回值
std::uninitialized_fill_n();//有返回值
*/

_YLH_BEGIN
//POD类型，执行fill
template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator _uninitialized_copy_solve(InputIterator first, InputIterator last,
	ForwardIterator result, __false_type)
{
	return std::copy(first, last, result);//结束后返回
}

//非POD类型，执行拷贝构造
template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator _uninitialized_copy_solve(InputIterator first, InputIterator last,
	ForwardIterator result, __true_type)
{
	ForwardIterator cur = result;
	try
	{
		for (; first != last; ++first, ++cur)
		{
			construct(&*cur, *first);
		}
		return cur;//返回操作完成后的迭代器的位置
	}
	catch (...)
	{
		std::cerr << __FUNCTION__ << '\n';
		destroy(result, cur);//commit or rallback原则
		throw;
	}
}

template <typename InputIterator, typename ForwardIterator,typename T>
inline ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last,
	ForwardIterator result, T*)
{
	using IS_POD_TYPE = typename __type_traits<T>::is_POD_type;
	return _uninitialized_copy_solve(first, last, result, IS_POD_TYPE());
}


template <typename InputIterator,typename ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
	return _uninitialized_copy(first, last, result, value_type(first));
}

//----------------------------------

//是POD类型
template <typename ForwardIterator, typename Value>
inline void _uninitialized_fill_solve(ForwardIterator first, ForwardIterator last, const Value& value, __true_type)
{
	std::fill(first, last, value);
}

//不是POD类型
template <typename ForwardIterator, typename Value>
inline void _uninitialized_fill_solve(ForwardIterator first, ForwardIterator last, const Value& value,__false_type)
{
	ForwardIterator cur = first;
	try
	{
		for (; first != last; ++cur)
		{
			construct(&*cur, value);
		}
	}
	catch (...)
	{
		std::cerr << __FUNCTION__ << '\n';
		destroy(first, cur);
	}
	
}

template <typename ForwardIterator, typename Value,typename T>
inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const Value& value, T*)
{
	using IS_POD_TYPE = typename __type_traits<T>::is_POD_type;
	_uninitialized_fill_solve(first, last, value, IS_POD_TYPE());
}

template <typename ForwardIterator, typename Value>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const Value& value)
{
	_uninitialized_fill(first, last, value, value_type(first));
}

//-------------------------------------

//POD类型
template <typename ForwardIterator, typename Size, typename Value>
inline ForwardIterator _uninitialized_fill_n_solve(ForwardIterator first, Size n, const Value& value, __true_type)
{
	return std::fill_n(first, n, value);
}

//非POD类型
template <typename ForwardIterator, typename Size, typename Value>
inline ForwardIterator _uninitialized_fill_n_solve(ForwardIterator first, Size n, const Value& value,__false_type)
{
	ForwardIterator cur = first;
	try
	{
		for (; n > 0; --n)
		{
			construct(&*cur, value);
		}
		return cur;
	}
	catch (...)
	{
		std::cerr << __FUNCTION__ << '\n';
		destroy(first, cur);
		throw;
	}
}


template <typename ForwardIterator, typename Size, typename Value,typename T>
inline ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, const Value& value,T*)
{
	using IS_POD_TYPE = typename __type_traits<T>::is_POD_type;
	return _uninitialized_fill_n_solve(first, n, value, IS_POD_TYPE());
}



template <typename ForwardIterator, typename Size, typename Value>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const Value& value)
{
	return _uninitialized_fill_n(first, n, value, value_type(first));
}


_YLH_END

#endif // !Y_UNINITIALIZED_H_

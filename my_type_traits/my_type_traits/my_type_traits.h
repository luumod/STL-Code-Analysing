#ifndef MY_TYPE_TRAITS_H
#define MY_TYPE_TRAITS_H
#include <iostream>
struct __true_type {};
struct __false_type {};

template <typename type>
struct __type_traits
{
	using has_trivial_default_constructor = __false_type;
	using has_trivial_copy_constructor = __false_type;
	using has_trivial_assignment_operator = __false_type;
	using has_trivial_destructor = __false_type;
	using is_POD_type = __false_type;
};


struct __type_traits<char>
{
	using has_trivial_default_constructor = __true_type;
	using has_trivial_copy_constructor = __true_type;
	using has_trivial_assignment_operator = __true_type;
	using has_trivial_destructor = __true_type;
	using is_POD_type = __true_type;
};
	

struct __type_traits<signed char>
{
	using has_trivial_default_constructor = __true_type;
	using has_trivial_copy_constructor = __true_type;
	using has_trivial_assignment_operator = __true_type;
	using has_trivial_destructor = __true_type;
	using is_POD_type = __true_type;
};


struct __type_traits<unsigned char>
{
	using has_trivial_default_constructor = __true_type;
	using has_trivial_copy_constructor = __true_type;
	using has_trivial_assignment_operator = __true_type;
	using has_trivial_destructor = __true_type;
	using is_POD_type = __true_type;
};


struct __type_traits<short>
{
	using has_trivial_default_constructor = __true_type;
	using has_trivial_copy_constructor = __true_type;
	using has_trivial_assignment_operator = __true_type;
	using has_trivial_destructor = __true_type;
	using is_POD_type = __true_type;
};


struct __type_traits<unsigned short>
{
	using has_trivial_default_constructor = __true_type;
	using has_trivial_copy_constructor = __true_type;
	using has_trivial_assignment_operator = __true_type;
	using has_trivial_destructor = __true_type;
	using is_POD_type = __true_type;
};

struct __type_traits<int>
{
	using has_trivial_default_constructor = __true_type;
	using has_trivial_copy_constructor = __true_type;
	using has_trivial_assignment_operator = __true_type;
	using has_trivial_destructor = __true_type;
	using is_POD_type = __true_type;
};


struct __type_traits<unsigned int>
{
	using has_trivial_default_constructor = __true_type;
	using has_trivial_copy_constructor = __true_type;
	using has_trivial_assignment_operator = __true_type;
	using has_trivial_destructor = __true_type;
	using is_POD_type = __true_type;
};


struct __type_traits<long>
{
	using has_trivial_default_constructor = __true_type;
	using has_trivial_copy_constructor = __true_type;
	using has_trivial_assignment_operator = __true_type;
	using has_trivial_destructor = __true_type;
	using is_POD_type = __true_type;
};

struct __type_traits<unsigned long>
{
	using has_trivial_default_constructor = __true_type;
	using has_trivial_copy_constructor = __true_type;
	using has_trivial_assignment_operator = __true_type;
	using has_trivial_destructor = __true_type;
	using is_POD_type = __true_type;
};

struct __type_traits<float>
{
	using has_trivial_default_constructor = __true_type;
	using has_trivial_copy_constructor = __true_type;
	using has_trivial_assignment_operator = __true_type;
	using has_trivial_destructor = __true_type;
	using is_POD_type = __true_type;
};

struct __type_traits<double>
{
	using has_trivial_default_constructor = __true_type;
	using has_trivial_copy_constructor = __true_type;
	using has_trivial_assignment_operator = __true_type;
	using has_trivial_destructor = __true_type;
	using is_POD_type = __true_type;
};

struct __type_traits<long double>
{
	using has_trivial_default_constructor = __true_type;
	using has_trivial_copy_constructor = __true_type;
	using has_trivial_assignment_operator = __true_type;
	using has_trivial_destructor = __true_type;
	using is_POD_type = __true_type;
};

template <typename T>
struct __type_traits<T*>
{
	using has_trivial_default_constructor = __true_type;
	using has_trivial_copy_constructor = __true_type;
	using has_trivial_assignment_operator = __true_type;
	using has_trivial_destructor = __true_type;
	using is_POD_type = __true_type;
};

//-----------------------------------------------
// 测试
//-----------------------------------------------

template <typename ForwardIterator, typename Size, typename T>
ForwardIterator __uninitialized_fill_n_anx(ForwardIterator first, Size n,
	const T& x, __false_type)
{
	//__false_type : 不是POD类型，进行对象的构造
	ForwardIterator cur = first;
	for (; n > 0; n--, ++cur)
	{
		construct(&*cur, x);
	}
	return cur;
}

template <typename ForwardIterator, typename Size, typename T>
ForwardIterator __uninitialized_fill_n_anx(ForwardIterator first, Size n,
	const T& x, __true_type)
{
	//__true_type：是POD类型，可以进行快速的操作，操作内存
	return std::fill_n(first, n, x);
}


template <typename ForwardIterator,typename Size,typename T,typename T1>
ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, const T1&)
{
	//T1为萃取出的迭代器所指的元素类型
	typename __type_traits<T1>::is_POD_type is_pod;
	return 
}

//从 first处开始构造n个元素
template <typename ForwardIterator,typename Size,typename T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
	//value_type利用 iterator_traits 萃取出迭代器所指的元素类型
	return __uninitialized_fill_n(first, n, x, value_type(first));
}


#endif // !MY_TYPE_TRAITS_H

#ifndef Y_CONSTRUCT_H_
#define Y_CONSTRUCT_H_
#include "Y_config.h"
#include "Y_type_traits.h"
#include "Y_iterator_traits.h"
/*
对象的构造与析构函数
*/
#include <new>

_YLH_BEGIN

//全局的析构函数
template <typename T>
inline void destroy(T* pointer)
{
	pointer->~T();//调用对象的析构函数
}

//全局的构造函数
template <typename T>
inline void construct(T* pointer, const T& value)
{
	new (pointer) T(value);//placement new操作
}

//POD类型:销毁对象元素
template <typename ForwardIteraor>
inline void _destroy_solve(ForwardIteraor first, ForwardIteraor last, __true_type&) {}

//非POD类型:调用析构函数销毁对象元素
template <typename ForwardIteraor>
inline void _destroy_solve(ForwardIteraor first, ForwardIteraor last, __false_type&)
{
	for (; first != last; first++)
	{
		destroy(&*first);
	}
}

template <typename ForwardIterator,typename T>
inline void _destroy(ForwardIterator first, ForwardIterator last,T*)//value_type返回T*
{
	using Trivial_dtor = typename __type_traits<T>::has_trivial_destructor;
	Trivial_dtor temp{};
	T type{};
	_destroy_solve(first, last, temp);
}

//销毁[first，last)中的元素
template <typename ForwardIterator>//可读可写迭代器
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	_destroy(first, last, value_type(first));//推断出迭代器的元素类型
}

//特化版本
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}

_YLH_END

#endif // !YCONSTRUCT_H_

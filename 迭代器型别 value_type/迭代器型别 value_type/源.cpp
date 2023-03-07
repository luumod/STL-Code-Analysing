#include <iostream>

//1. tempalte的参数类型推导
template <typename Iter, typename T>
void func_temp(Iter it, T t)
{
	//it: int* ; t: int
	T tmp{}; //tmp: int
}

template <typename T>
void func(T val)
{
	func_temp(val, *val);//val: int* ;  *val: int
}

//------------------------------------------------
//2. 内嵌类型
template <typename T>
struct Mystu
{
	typedef T value_type;	//内嵌型别
	T* ptr;
	Mystu(T* ptr = NULL) :ptr(ptr) {}
	T& operator*()const { return *ptr; }
};

template <typename T>
typename T::value_type func2(T ite)
{//自动推导出 T 属于 Mystu<int>类型，从而返回Mystu<int>的 value_type，即T类型
	//T的型别是 ite的型别即 Mystu<int> 但是作为返回值的类型是什么呢？
	return *ite;//解引用获得其值
}

//------------------------------------------------
//3. 萃取迭代器的类型
template <typename T>
struct iterator_traits 
{
	//如果 T 有自己的value_type 那么通过这个traits，萃取出来的value_type 就是T的value_type
	typedef typename T::value_type value_type;
};
template <typename T>
typename iterator_traits<T>::value_type func3(T ite)
{
	//iterator_traits<Mystu<int>> -> value_type = Mystu<int>::value_type -> int
	return *ite;
}

// traits的特化版本 非const
template <typename T>
struct iterator_traits<T*>
{
	//指针类型，优先匹配此迭代器
	typedef T value_type; 
	//传递过程： T*=int*  ->  value_type=int  ->  iterator_traits<int*>::value_type=int
};
// traits的特化版本 const版本
template <typename T>
struct iterator_traits<const T*>
{
	//const 指针类型，优先匹配此迭代器
	typedef T value_type;
	//传递过程： const T*=const int*  ->  value_type=int  -> iterator_traits<const int*>::value_type=int
};


int main()
{
	/*int a = 10;
	func(&a);*/

	//Mystu<int> b{ new int{10} };//非指针
	int* c = new int{ 10 };//原生指针类型
	const int* d = new int{ 20 };
	std::cout << func3(d);

	return 0;
}
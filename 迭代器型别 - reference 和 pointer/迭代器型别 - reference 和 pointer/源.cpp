#include <iostream>

template <typename T>
struct iterator_traits
{
	typedef typename T::pointer pointer;
	typedef typename T::reference reference;
};
template <typename T>
struct iterator_traits<T*>
{
	typedef T* pointer;
	typedef T& reference;
};
template <typename T>
struct iterator_traits
{
	typedef const T* pointer;
	typedef const T& reference;
};

int main()
{


	return 0;
}
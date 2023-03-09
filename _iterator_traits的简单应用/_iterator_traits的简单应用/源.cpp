#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#define NO_ITERATOR_TRAITS
//#undef NO_ITERATOR_TRAITS

/*
把同一类型的容器中的元素全部拷贝到另一个容器并且排序，然后输出：
1. 普通做法：
	1.1  Mycopy函数《显式传递》迭代器所指的元素类型，然后再传入三个迭代器，然后执行操作
	1.2  show函数《显示传递》迭代器所指的元素类型，然后再传入两个迭代器，然后执行操作
2. iterator_traits做法：
	2.1  Mycopy只需传入三个迭代器即可，在其内部进行迭代器的《萃取工作》 无需显式传递元素类型
	2.1  show只需传入两个迭代器即可，在其内部进行迭代器的《萃取工作》 无需显式传递元素类型

总结： iterator_traits提供了在操作时无需传递显式指定元素类型的操作，更加符合STL的规范
*/


//把src的元素复制到dst中，并且对dst里的元素排序
#if !defined(NO_ITERATOR_TRAITS)
template <typename T,typename InputIterator, typename OutputIterator>
void Mycopy(InputIterator beg, InputIterator end, OutputIterator dst)
{
	//手动指定迭代器所指元素类型
	std::vector<T> rets;
	while (beg != end)
	{
		rets.push_back(*beg);
		beg++;
	}
	std::sort(rets.begin(), rets.end());
	std::copy(rets.begin(), rets.end(), dst);
}
template <typename T,typename InputIterator>
void show(InputIterator beg, InputIterator end)
{
	std::copy(beg, end, std::ostream_iterator<T>(std::cout, " "));
	std::cout << '\n';
}
#else
template <typename T>
struct iterator_traits
{
	using value_type = typename T::value_type;
	//.....
};
template <typename InputIterator,typename OutputIterator>
void Mycopy(InputIterator beg, InputIterator end, OutputIterator dst)
{
	std::vector<typename iterator_traits<InputIterator>::value_type> rets;
	while (beg != end)
	{
		rets.push_back(*beg);
		++beg;
	}
	std::sort(rets.begin(), rets.end());
	std::copy(rets.begin(), rets.end(), dst);
}
template <typename InputIterator>
void show(InputIterator beg, InputIterator end)
{
	std::copy(beg, end, std::ostream_iterator<typename iterator_traits<InputIterator>::value_type>(std::cout, " "));
	std::cout << '\n';
}
#endif
int main()
{
	std::vector<int> vec1{ 9,8,7,6,5,2,8,9,4,11 },dst1(vec1.size());
	std::vector<double> vec2{ 9.1,8.1,7.6,6.9,5.5,2.4,8.6,9.2,4.8,1.1 }, dst2(vec2.size());
	std::vector<char> vec3{ 'a','b','A','P','z','L'}, dst3(vec3.size());
	std::vector<std::string> vec4{ "woda","pod","abcd","bb"}, dst4(vec4.size());
#if !defined(NO_ITERATOR_TRAITS)
	Mycopy<int>(vec1.begin(), vec1.end(), dst1.begin());
	Mycopy<double>(vec2.begin(), vec2.end(), dst2.begin());
	Mycopy<char>(vec3.begin(), vec3.end(), dst3.begin());
	Mycopy<std::string>(vec4.begin(), vec4.end(), dst4.begin());

	show<int>(vec1.begin(), vec1.end());
	show<double>(vec2.begin(), vec2.end());
	show<char>(vec3.begin(), vec3.end());
	show<std::string>(vec4.begin(), vec4.end());
#else
	Mycopy(vec1.begin(), vec1.end(), dst1.begin());
	Mycopy(vec2.begin(), vec2.end(), dst2.begin());
	Mycopy(vec3.begin(), vec3.end(), dst3.begin());
	Mycopy(vec4.begin(), vec4.end(), dst4.begin());

	show(vec1.begin(), vec1.end());
	show(vec2.begin(), vec2.end());
	show(vec3.begin(), vec3.end());
	show(vec4.begin(), vec4.end());
#endif
	return 0;
}
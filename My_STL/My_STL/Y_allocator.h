#ifndef Y_ALLOCATOR_H_
#define Y_ALLOCATOR_H_
#include "Y_config.h"
#include <cstdlib>
#include <malloc.h>
/*
空间配置器
*/
_YLH_BEGIN

//空间配置器接口
template <typename T,typename Alloc>
class simplae_alloc
{
public:
	static T* allocate(size_t n)
	{
		//多个空间
		return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	}
	static T* allocate(void)
	{
		//一个空间
		return (T*)Alloc::allocate(sizeof(T));
	}
	static void deallocate(T* p)
	{
		Alloc::deallocate(p, sizeof(T));
	}
	static void deallocate(T* p,size_t n)
	{
		if (n != 0) Alloc::deallocate(p, n * sizeof(T));
	}
};


//第一级空间配置器
template <typename Null> //注意此模板参数没有用到
class malloc_alloc_template
{
private:
	static void* oom_malloc(size_t n);
	static void* oom_realloc(void* p, size_t n);
	static void (*malloc_alloc_template_handler)();
public:
	static void* allocate(size_t n)
	{
		void* start = malloc(n);//直接使用malloc即可
		if (start == nullptr)
		{
			//处理oom
		}
		return start;
	}
	static void deallocate(void* d,size_t)
	{
		free(d);
	}
	static void* reallocate(void* p, size_t n_objs)
	{
		void* start = realloc(start, n_objs);
		if (start == nullptr)
		{
			//处理oom
		}
		return start;
	}
private:
};

//类成员变量的初始化
template <typename Null>
void (*malloc_alloc_template<Null>::malloc_alloc_template_handler)() = nullptr;
//处理malloc和realloc分配内存不足的情况
template<typename T>
inline void* malloc_alloc_template<T>::oom_malloc(size_t n)
{
	void (*m_malloc_handler)();//函数指针
	void* result;

	while (true)
	{
		m_malloc_handler = malloc_alloc_template_handler;
		if (m_malloc_handler == NULL)
		{
			THROW_BAD_ALLOC
		}
		(*m_malloc_handler)();//尝试调用此函数
		result = malloc(n);
		if (result) return result;
	}
}
template<typename T>
inline void* malloc_alloc_template<T>::oom_realloc(void* p, size_t n)
{
	void (*m_realloc_handle)();
	void* result;

	while (true)
	{
		m_realloc_handle = malloc_alloc_template_handler;
		if (m_realloc_handle == NULL)
		{
			THROW_BAD_ALLOC
		}
		(*m_realloc_handle)();//尝试调用此函数
		result = realloc(p, n);
		if (result) { return result; }
	}
}

struct Null {};
using malloc_alloc = malloc_alloc_template<Null>; //第一级空间配置器

//第二级空间配置器
template <typename Null>
class default_alloc_template
{
private:
	enum { __ALIGN = 8 };//对齐的个数
	enum { __MAX_BYTES = 128 };//小型区块的个数
	enum { __NUM_FREE_LISTS = __MAX_BYTES / __ALIGN };//自由链表的个数
private:
	union Area
	{
		Area* next;
	};
	//自由链表的指针数组：16个自由链表，每个自由链表管理8个字节
	static Area* volatile free_list[__NUM_FREE_LISTS];
	static char* end_free;
	static char* start_free;
	static size_t heap_size;
private:
	//将bytes上调至8的倍数
	static size_t ROUND_UP(size_t bytes) {
		return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
	}
	//决定使用第几号 free_list  从1开始
	static  size_t FREELIST_INDEX(size_t bytes) {
		return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
	}
	//重新填充自由链表
	static void* refill(size_t n)
	{
		int nobjs = 20;
		//nobjs通过传递引用的方式 来取得nobjs个区块作为free_list的新节点
		char* chunk = chunk_alloc(n, nobjs);

		Area* volatile* my_free_list;
		Area* result;
		Area* current_obj, * next_obj;
		int i;

		if (1 == nobjs) return(chunk);//只获得一个区块
		//多个区块：调整free_list  纳入新节点
		my_free_list = free_list + FREELIST_INDEX(n);

		//在chunk空间里建立free_List 
		result = (Area*)chunk;
		*my_free_list = next_obj = (Area*)(chunk + n);//free_list指向新配置的空间（取自内存池）

		//将free_list各个节点串联起来
		for (i = 1; ; i++) {
			current_obj = next_obj;
			next_obj = (Area*)((char*)next_obj + n);
			if (nobjs - 1 == i) {
				current_obj->next = nullptr;
				break;
			}
			else {
				current_obj->next = next_obj;
			}
		}
		return (result);
	}
	//内存池
	static char* chunk_alloc(size_t size, int& nobjs)
	{
		char* result;
		size_t total_bytes = size * nobjs;//总的需要分配的大小  *20倍
		size_t pool_bytes_left = end_free - start_free;//内存池剩余空间

		if (pool_bytes_left >= total_bytes) {
			//内存池剩余容量充足 可以容纳20个节点
			result = start_free;//起始
			start_free += total_bytes;//total_bytes全部分配
			return(result);
		}
		else if (pool_bytes_left >= size) {
			//内存池剩余容量不能容纳全部，但是可以容纳 1个 及以上的块的空间
			//nobjs是引用类型，修改为实际能够供应的区块数
			nobjs = pool_bytes_left / size;//最大能容纳的n
			total_bytes = size * nobjs;//最大能分配的空间
			result = start_free;//起始地址空间
			start_free += total_bytes;//最多能容纳的total_bytes全部分配
			return(result);
		}
		else {
			//一个块都容纳不了！ 则尝试heap中配置
			//新的空间大小为原始需要分配的空间大小的两倍

			//从heap上分配的空间大小： 2*所需节点空间大小*20+额外空间
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			//内存池还有一点剩余，但是不够一个节点的空间
			if (pool_bytes_left > 0) {
				Area* volatile* my_free_list = free_list + FREELIST_INDEX(pool_bytes_left);

				//将内存池中的剩余空间全部编入
				((Area*)start_free)->next = *my_free_list;
				*my_free_list = (Area*)start_free;
			}

			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free) {//malloc失败，说明heap上没有足够空间分配给我们了
				Area* volatile * my_free_list, * p;
				for (int i = size; i <= (size_t)__MAX_BYTES; i += (size_t)__ALIGN) {
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (0 != p) {
						*my_free_list = p->next;
						start_free = (char*)p;
						end_free = start_free + i;
						//递归调用自己，修复nobjs
						return(chunk_alloc(size, nobjs));
					}
				}
				/*
				一点内存都没有了
				*/
				end_free = 0;	// In case of exception.
				//最后调用第一级配置器，看看能否得到改善
				start_free = (char*)malloc_alloc::allocate(bytes_to_get);
			}
			heap_size += bytes_to_get;//调整堆空间
			end_free = start_free + bytes_to_get;
			return (chunk_alloc(size, nobjs));
		}
	}

public:
	static void* allocate(size_t n)
	{
		Area* volatile * my_free_lists;//二级指针
		Area* result;
		if (n > (size_t)__MAX_BYTES)
		{
			return malloc_alloc::allocate(n);//转而调用第一级配置器
		}
		my_free_lists = free_list + FREELIST_INDEX(n);//决定使用第几号free-list
		result = *my_free_lists;
		if (result == NULL)
		{
			void* r = refill(ROUND_UP(n));
			return r;
		}
		*my_free_lists = result->next;
		return result;
	}
	static void* reallocate(void* p,size_t old_sz,size_t new_sz)
	{
		void* result;
		size_t copy_sz;

		if (old_sz > (size_t)__MAX_BYTES && new_sz > (size_t)__MAX_BYTES) {
			return (realloc(p, new_sz));
		}
		if (ROUND_UP(old_sz) == ROUND_UP(new_sz)) return(p);
		result = allocate(new_sz);
		copy_sz = new_sz > old_sz ? old_sz : new_sz;
		memcpy(result, p, copy_sz);
		deallocate(p, old_sz);
		return(result);
	}
	static void deallocate(void* p, size_t n)
	{
		Area* q = (Area*)p;
		Area* volatile* my_free_list;

		//大于128调用第一级配置器
		if (n > (size_t)__MAX_BYTES) {
			malloc_alloc::deallocate(p, n);
			return;
		}
		my_free_list = free_list + FREELIST_INDEX(n);//寻找对应的free_list
		//调整list 回收区块
		q->next = *my_free_list;
		*my_free_list = q;
	}
};

//静态数据成员的初始化
char* default_alloc_template<Null>::start_free = 0;
char* default_alloc_template<Null>::end_free = 0;
size_t default_alloc_template<Null>::heap_size = 0;
default_alloc_template<Null>::Area* volatile default_alloc_template<Null>::free_list[
		default_alloc_template<Null>::__NUM_FREE_LISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

//alloc为默认的第二级空间配置器
using alloc = default_alloc_template<Null>;

_YLH_END


#endif

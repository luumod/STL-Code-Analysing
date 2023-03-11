#ifndef Y_VECTOR_H_
#define Y_VECTOR_H_
#include "Y_memory.h"

_YLH_BEGIN


template <typename T, typename Alloc = alloc>//空间Alloc器
class vector
{
public:
	//相应类型
	using value_type = T;

	using iterator = value_type*;
	using const_iterator = value_type*;

	using pointer = value_type*;
	using const_pointer = const value_type*;

	using reference = value_type&;
	using const_reference = const value_type&;

	using size_type = size_t;
	using difference_type = ptrdiff_t;
protected:
	using data_allocator = simplae_alloc<value_type, Alloc>;//vector默认使用第二级空间配置器
public: //构造函数
	vector():start(0), finish(0), end_of_storage(0) {}
	//初始化列表
	vector(std::initializer_list<value_type> list) {
		int total_size = list.size();
		for (const value_type x : list) {
			push_back(x);
		}
	}
	//n个元素为val值
	vector(size_type n, const value_type& val) {
		fill_initialize(n, val); 
	}
	//析构函数
	~vector()
	{
		destroy(start, finish);
		vector_deallocate();
	}
public://基本操作函数
	inline iterator begin() {
		return start; 
	}
	inline const_iterator cbegin()const {
		return start; 
	}
	inline iterator end() {
		return finish;
	}
	inline const_iterator cend()const {
		return finish; 
	}
	inline size_type size()const {
		_TRY{
			return _size(); 
		}
		_CATCH(...) {
			std::cerr << NULL_vector;
			EXIT(0);
		}
	}
	inline size_type capacity()const {
		return _capacity();
	}
	inline bool empty()const {
		return cbegin() == cend();
	}
	inline reference operator[](size_type n) {
		if (empty() ||n>=size()) {
			std::cout << NULL_vector;
			EXIT(0);
		}
		return *(begin() + n); 
	}
	inline reference front() {
		_TRY{ 
			return _front(); 
		}
		_CATCH(...) {
			std::cerr << NULL_vector;
			EXIT(0);
		}
	}
	inline reference back() {
		_TRY {
			return _back();
		}
		_CATCH(...) {
			std::cerr << NULL_vector;
			EXIT(0);
		}
	}
	//得到某个位置的迭代器
	iterator get(int n) {
		iterator cur = start;
		for (; n > 0; n--, cur++) {}
		return cur;
	}
public:
	//弹出最后一个元素
	inline void pop_back() { 
		if (empty()) {
			std::cerr << NULL_vector;
			EXIT(0);
		}
		finish--; 
		destroy(finish); 
	}
	//消除[first,last)中的元素
	iterator erase(iterator first, iterator last)
	{
		if (empty()) {
			std::cerr << NULL_vector;
			throw NULL;
		}
		//将[last,finish)中的元素全部拷贝到first处的位置
		iterator end_pos = std::copy(last, finish, first);
		destroy(end_pos, finish);//清除[end_pos,finish)元素空间
		finish = finish - (last - first);
		return first;
	}
	//消除position位置的元素
	iterator erase(iterator position)
	{
		if (position + 1 != end())
		{
			std::copy(position + 1, finish, position);
		}
		destroy(finish);//随便销毁一个元素
		finish -= 1;//随便减少1个元素大小
		return position;
	}
	//清除所有元素
	inline void clear() 
	{ 
		erase(begin(), end()); 
	}
	//尾插元素
	void push_back(const value_type& x) {
		if (finish != end_of_storage) {
			::construct(&*finish, x);
			++finish;
		}
		else {
			//扩容 + 插入
			insert_solve(end(), x);
		}
	}
	//中间插入一个元素
	void insert(iterator position, const value_type& val) {
		 insert_solve(position, val);
	}
	//一次性插入n个val元素
	void insert(iterator position,const size_type n, const value_type& val) {
		size_type elem_after_pos = finish - position;//在pos位置后finish之前的元素个数
		if (n <= (size_type)(end_of_storage - finish)) {//剩余的空间足够容纳n个元素
			//无需扩容
			iterator old_finish = finish;
			if (elem_after_pos > n) {
				finish = uninitialized_copy(finish - n , finish, finish);
				 //+= elem_after_pos - n;;
				std::copy_backward(position, old_finish - n, old_finish);
				std::fill(position, position + n, val);
			}
			else {
				uninitialized_fill_n(finish, n - elem_after_pos, val);
				finish += n - elem_after_pos;
				uninitialized_copy(position, old_finish, finish);
				finish += elem_after_pos;
				std::fill(position, old_finish, val);
			}
		}
		else {
			//需要扩容
			const size_type old_size = size();
			//总的需要的元素个数
			const size_type total_size = old_size + std::max(old_size, n);
			iterator new_start = data_allocator::allocate(total_size);
			iterator new_finish = new_start;
			_TRY{
				new_finish =  uninitialized_copy(start,position,new_start);
				new_finish =  uninitialized_fill_n(new_finish, n, val);
				new_finish =  uninitialized_copy(position, finish, new_finish);
			}
			_CATCH(...){
				//失败，rallback
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, total_size);
			}
			destroy(start, finish);
			vector_deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + total_size;
		}
	}
private://DBG
	inline const_iterator cend_of_storage()const {
		return end_of_storage;
	}
	inline size_type _capacity()const {
		return size_type(cend_of_storage() - cbegin());
	}
	inline reference _front() {
		if (empty()) 
			throw NULL_VECTOR;
		return *begin();
	}
	inline reference _back() {
		if (empty()) 
			throw NULL_VECTOR;
		return *(end() - 1);
	}	
	inline size_type _size()const
	{
		return size_type(cend() - cbegin());
	}
	//调整空间
	void fill_initialize(size_type n, const value_type& val)
	{
		start = allocate_and_fill(n, val);//起始地址
		finish = begin() + n;//元素结束地址
		end_of_storage = finish;//总容量结束地址
	}
	//分配空间与填充
	iterator allocate_and_fill(size_type n, const value_type& val)
	{
		iterator res = data_allocator::allocate(n);//开辟n个元素的空间
		uninitialized_fill_n(res, n, val);//res开始填充n个val
		return res;//返回此空间的起始地址
	}
	void insert_solve(iterator position, const value_type& val) {
		//足够插入，在position位置插入一个元素
		if (finish != end_of_storage) {
			::construct(&*finish, *(finish - 1));//先构造一个空间
			++finish;
			std::copy(position, finish - 1, position + 1);
			*position = val;
			return;
		}
		else {
			//空间不够，开内存
			const size_type cur_size = size(); //现有的元素个数
			const size_type total_size =  (0 == cur_size) ? 1: cur_size << 1;//开两倍内存
			iterator new_start = data_allocator::allocate(total_size);
			iterator new_finish = new_start;
			_TRY{
				//position之前的拷贝过来
				new_finish =  uninitialized_copy(start,position ,new_start);//返回结束位置
				::construct(&*new_finish, val);//构造一个元素
				++new_finish;
				uninitialized_copy(position, finish, new_finish);
			}
			_CATCH(...){
				//commit or rallbak
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, total_size);
				throw NULL_VECTOR;
			}
			//销毁原始空间
			destroy(start, finish);
			vector_deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + total_size;
		}
	}
	//vector的销毁函数
	void vector_deallocate(){
		if (start){
			data_allocator::deallocate(start, end_of_storage - start);
		}
	}
private:
	iterator start;
	iterator finish;
	iterator end_of_storage;
};

_YLH_END


#endif // !Y_VECTOR_H_


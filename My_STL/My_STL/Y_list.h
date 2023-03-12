#ifndef Y_LIST_H_
#define Y_LIST_H_
#include "Y_config.h"
#include "Y_iterator_traits.h"
#include "Y_memory.h"
_YLH_BEGIN

//list的节点类型
template <typename T>
struct listNode{
	listNode<T>* next;
	listNode<T>* prev;
	T	data;
};

//list的迭代器类型
template <typename T,typename Ref,typename Ptr>
class list_iterator{
public:
	using iterator = list_iterator<T, T&, T*>;
	using self = list_iterator<T, Ref, Ptr>;

	using value_type = T;
	using difference_type = ptrdiff_t;
	using pointer = Ptr;
	using reference = Ref;
	using iterator_category = Bibirectional_Iterator_tag;//双向
	using size_type = size_t;
	using link_type = listNode<T>*;	
	//共有，方便迭代器获取node成员
	link_type node; //list节点
public://构造函数
	list_iterator() {}
	list_iterator(link_type x) :node(x) {}
	list_iterator(const iterator& x) :node(x.node) {}
	~list_iterator() {}
public:
	bool operator==(const self& lhs) {
		return lhs.node == node;
	}
	bool operator!=(const self& lhs) {
		return lhs.node != node;
	}
	//*运算，获取迭代器所指节点的值
	reference operator*() {
		return node->data;
	}
	//->运算，获取迭代器所指节点值的地址
	pointer operator->()const {
		return &(node->data);
	}
	//前置++
	self& operator++() {
		node = node->next;//前进到下一个
		return *this;
	}
	//后置++
	self operator++(int) {
		auto temp = *this;
		++* this;
		//返回一个临时temp，因此不能使用引用
		return temp;
	}
	//前置--
	self& operator--() {
		node = node->prev;
		return *this;
	}
	//后置--
	self operator--(int) {
		auto temp = *this;
		--* this;
		return temp;
	}
};

//list
template <typename T,typename Alloc=alloc>
class list
{
protected:
	using list_node = listNode<T>;
	using data_allocator = simple_alloc<list_node, Alloc>;
public:
	using link_type = list_node*;

	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using difference_type = ptrdiff_t;
	using size_type = size_t;
public:
	using iterator = list_iterator<value_type, reference, pointer>;
	using const_iterator = list_iterator<value_type, const_reference, const_pointer>;
private:
	link_type head;//头节点
public:
	inline iterator begin() {
		return head->next;//返回list_iterator迭代器类型
	}
	inline const_iterator cbegin()const {
		return head->next;
	}
	inline iterator end() {
		return head;//头节点不存储任何数据，它就是end节点
	}
	inline const_iterator cend()const {
		return head;
	}
	inline bool empty() {
		return head == head->next;//自己和自己连接，则list为空
	}
	inline size_type size() {
		return (size_type)distance(begin(), end());//计算两个迭代器之间的距离
	}
	//引用方式返回
	inline reference front() {
		return *begin();//head->next->data
	}
	inline reference back() {
		return *(--end());//list没有 - + 重载
	}
	inline void swap(list& ls) {
		std::swap(head, ls.head);
	}
public://内存配置
	//配置节点空间
	link_type create_node_space() {
		return (link_type)data_allocator::allocate();
	}
	//销毁节点空间
	void destroy_node_space(link_type pDel) {
		data_allocator::deallocate(pDel);
	}
	//创建节点对象
	link_type create_node(const value_type& val) {
		//commit or rallback规则
		link_type pNew = create_node_space();//分配空间
		_TRY{
			construct(&(pNew->data), val);//构造对象
		}
			_CATCH(...) {
			destroy_node_space(pNew);//否则销毁空间
		}
		return pNew;
	}
	//销毁节点对象
	void destroy_node(link_type pDel) {
		::destroy(&pDel->data);
		destroy_node_space(pDel);
	}
public://构造函数
	list() {
		empty_initialized();
	}
	list(const std::initializer_list<value_type>& ls) {
		empty_initialized();
		for (value_type val : ls) {
			push_back(val);
		}
	}
public://对元素的操作
	//尾插法
	void push_back(const value_type& value) {
		//其实就是在end的位置插入
		_insert(end(), value);
	}
	//头插法
	void push_front(const value_type& value) {
		_insert(begin(), value);
	}
	//删除头节点
	void pop_front() {
		erase(begin());
	}
	//删除尾节点
	void pop_back() {
		erase(--end());
	}
	//删除position处的节点，返回删除后的当前位置的节点
	iterator erase(const const_iterator position) {
		link_type pDel = position.node;
		link_type pDelNext = pDel->next;
		link_type pDelPrev = pDel->prev;
		pDelPrev->next = pDelNext;
		pDelNext->prev = pDelPrev;
		destroy_node(pDel);//销毁pDel节点
		return pDelNext;
	}
	//清空链表
	void clear() {
		link_type cur = begin().node, temp = nullptr;
		link_type end = head;//尾节点
		while (cur != end) {
			temp = cur;
			cur = cur->next;
			destroy_node(temp);
		}
		//恢复空list状态
		cur->next = cur;
		cur->prev = cur;
	}
	//删除所有值为value的元素
	void remove(const value_type& value) {
		iterator cur = begin(), temp = nullptr;
		while (cur != end()) {
			temp = cur.node->next;
			if (cur.node->data == value) {
				erase(cur);
			}
			cur = temp;
		}
	}
	//移除连续且相同元素的节点，只剩下一个
	void unique() {
		iterator first = begin(), last = end();
		iterator next = first;
		while (++next != last) {
			//first始终指向某个元素的第一个位置
			//变动删除next达到只剩下一个first
			if (*first == *next) {
				erase(next);
			}
			else {
				first = next;//first往后移动
			}
			next = first;
		}
	}
	//将ls接在position之前，ls必须不同于*this
	void splice(iterator position, list& ls) {
		if (!ls.empty()) {
			transform(ls.begin(), ls.end(), position);
		}
	}
	//将某一个迭代器接在position之前,position和it属于同一个list
	void splice(iterator position,list&,iterator it) {
		iterator it_ = it;
		++it_;//last
		if (it == position || it_ == position) {
			return;
		}
		transform(it, it_, position);
	}
	//将 [first,last)所有元素接在position之前
	void splice(iterator position, iterator first,iterator last) {
		if (first != last) {
			transform(first, last, position);
		}
	}
	//将ls的list合并到*this中，ls会消失，两个list必须有序！
	void merge(list& ls) {
		iterator first1 = begin();
		iterator first2 = ls.begin();
		iterator end1 = end();
		iterator end2 = ls.end();
		//合并到 first
		while (first1 != end1 && first2 != end2) {
			if (*first1 > *first2) {
				iterator temp = first2;
				transform(first2, ++temp, first1);
				first2 = temp;//移动到下一个
			}
			else {
				++first1;
			}
		}
		//待合并的ls还有，则全部放后面
		if (first2 != end2) {
			transform(first2, end2, end1);
		}
	}
	//翻转链表
	void reverse() {
		//如果NULL或者只有1个，则不执行
		if (head->next == head || head->next->next == head) {
			return;
		}
		iterator first = begin(),last=end();
		++first;//跳过head
		while (first != last) {
			auto temp = first;
			transform(temp,++first, begin());
		}
	}
	//遍历整个链表
	void show() {
		iterator first = begin();
		iterator last = end();
		for (; first != last; ++first) {
			std::cout << *first << " ";
		}
		std::cout << std::endl;
	}
	//排序
	void sort() {
		if (head->next == head || head->next->next == head) return;
		list<T, Alloc> carry;//每一归并层之间合并的 “中转站”
		list<T, Alloc> counter[64]; // counter[i]表示第i层《归并层》
		int fill = 0;
		while (!empty()) {//一直输入元素
			carry.splice(carry.begin(), *this, begin());//每次carry首先获取新插入的元素
			int i = 0;
			/*
			每一层从 0->i 归并层进行逐一合并
			*/
			while (i < fill && !counter[i].empty()) {
				counter[i].merge(carry);		//首先把carry 合并到 counter[i]层
				carry.swap(counter[i++]);	//交由carry临时存储此层归并后的结果
			}
			carry.swap(counter[i]); //将当前处理的结果给到 counnter[i] 层
			if (i == fill) {	//归并层扩容
				++fill;
			}
		}
		for (int i = 1; i < fill; ++i) {
			counter[i].merge(counter[i - 1]); //层层归并
		}
		swap(counter[fill - 1]);//最后一层就是答案
	}
private:
	//头节点的配置：创建一个空的list
	void empty_initialized() {
		head = create_node_space();
		head->next = head;
		head->prev = head;
	}
	//position位置创建并且插入一个节点，返回插入完成后的新的插入位置
	link_type _insert(iterator position, const value_type& value) {
		link_type pNew = create_node(value);//创建节点
		//中间插入
		pNew->next = position.node;
		pNew->prev = position.node->prev;
		position.node->prev->next = pNew;
		position.node->prev = pNew;
		return pNew;//
	}
	//将[first,last) 的全部元素移动到 position之前
	void transform(iterator first, iterator last, iterator position) {
		//[first,end]
		if (position != last) {
			iterator end = last.node->prev;//需要移动的最后一个元素
			iterator pos_prev = position.node->prev;
			iterator first_prev = first.node->prev;

			//next连接
			end.node->next = position.node;
			first_prev.node->next = last.node;
			pos_prev.node->next = first.node;

			//prev连接
			position.node->prev = last.node->prev;
			last.node->prev = first.node->prev;
			first.node->prev = pos_prev.node;
		}
	}
};

_YLH_END

#endif // !Y_LIST_H_

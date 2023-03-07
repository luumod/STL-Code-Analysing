#ifndef MT_LIST
#define MT_LIST

template <typename T>
class ListNode
{
public:
	ListNode() {}
	ListNode(T val, ListNode* pnext = nullptr)
		:_data(val), _next(pnext) {}
	T value()const { return this->_data; }
	ListNode* next()const { return this->_next; } //关键函数
	bool operator!=(const T& val)const { return this->_data!=val; }
	bool operator==(const T& val)const { return this->_data == val; }
	//int operator++() {  return 1; }
public:
	T _data;
	ListNode* _next;	//next指针
};

template <typename T>
class List
{
public:
	List() :_front(nullptr), _end(nullptr), _size(0) {}
	//单链表尾插
	void insert_end(T value)
	{
		if (this->_size == 0)
		{
			ListNode<T>* pNew = new ListNode<T>{ value };
			this->_end = pNew;
			this->_front = pNew;
			this->_size++;
			return;
		}
		ListNode<T>* pNew = new ListNode<T>{ value };
		this->_end->_next = pNew;
		this->_end = pNew;
		this->_size++;
	}
	//单链表头插
	void insert_front(T value)
	{
		ListNode<T>* pNew = new ListNode<T>(value);
		pNew->_next = this->_front;
		this->_front = pNew;
		this->_size++;
		if (this->_size == 1)
		{
			this->_end = this->_front;
		}
	}
	//遍历链表
	void show(std::ostream& out=std::cout)
	{
		ListNode<T>* temp = this->_front;
		while (temp!=nullptr)
		{
			out << temp->value() << " ";
			temp = temp->_next;
		}
		out << "\n";
	}
	std::ostream& operator<<(std::ostream& os) {}
public:
	ListNode<T>* front()const { return this->_front; }
	ListNode<T>* end()const { return this->_end; }
private:
	ListNode<T>* _end;
	ListNode<T>* _front;
	size_t _size;
};

#endif // !MT_LIST
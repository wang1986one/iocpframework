#ifndef __SLIST_HPP
#define __SLIST_HPP



namespace DataStructure
{

	// 单向链表的节点基本结构
	struct __SListNodeBase
	{
		__SListNodeBase *m_pNext;
	};


	// 创建新节点放于其后
	inline __SListNodeBase *__SListMakeLink(__SListNodeBase *pPrevNode, __SListNodeBase *pNewNode)
	{
		// 令New节点的下一结点为Prev节点的下一结点
		pNewNode->m_pNext	= pPrevNode->m_pNext;
		pPrevNode->m_pNext	= pNewNode;

		return pNewNode;
	}

	// 找出某节点的前一个节点
	inline __SListNodeBase *__SListPrev(__SListNodeBase *pHead, const __SListNodeBase *pNode)
	{
		while( pHead != NULL && pHead->m_pNext != pNode )
			pHead = pHead->m_pNext;

		return pHead;
	}

	// 找出某节点的前一个节点(const)
	inline const __SListNodeBase *__SListPrev(const __SListNodeBase *pHead, const __SListNodeBase *pNode)
	{
		while( pHead != NULL && pHead->m_pNext != pNode )
			pHead = pHead->m_pNext;

		return pHead;
	}

	inline void __SListSpliceAfter(__SListNodeBase *pPos, __SListNodeBase *pFirst, __SListNodeBase *pLast)
	{
		if( pPos != pFirst && pPos != pLast )
		{
			__SListNodeBase *pFirstTemp = pFirst->m_pNext;
			__SListNodeBase *pAfterTemp = pPos->m_pNext;

			pFirst->m_pNext = pLast->m_pNext;
			pPos->m_pNext	= pFirstTemp;
			pLast->m_pNext	= pAfterTemp;
		}
	}

	inline __SListNodeBase *__SListReverse(__SListNodeBase *pNode)
	{
		__SListNodeBase *pRes	= pNode;
		pNode					= pNode->m_pNext;
		pRes->m_pNext			= NULL;

		while(pNode != NULL)
		{
			__SListNodeBase *pNext	= pNode->m_pNext;
			pNode->m_pNext			= pRes;
			pRes					= pNode;
			pNode					= pNext;
		}
	}


	// 单项链表节点结构
	template<typename T>
	struct __SListNode
		: public __SListNodeBase
	{
		T m_data;
	};

	// 单向链表的迭代器基本结构
	struct __SlistIteratorBase
	{
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef ForwardIteratorTag iterator_category;	// 单向

	public:
		// 指向节点的基本结构
		__SListNodeBase *m_pNode;	

	public:
		__SlistIteratorBase(__SListNodeBase *pRhs)
			: m_pNode(pRhs)
		{}

	public:
		// 前进一个节点
		void Increment()
		{
			m_pNode = m_pNode->m_pNext;
		}

		bool operator==(const __SlistIteratorBase &rhs) const
		{
			return m_pNode == rhs.m_pNode;
		}
		bool operator!=(const __SlistIteratorBase &rhs) const
		{
			return !(*this == rhs);
		}
	};


	// 单向链表的迭代器结构
	template<typename T, typename Ref, typename Ptr>
	struct __SListIterator
		: public __SlistIteratorBase
	{
		typedef __SListIterator<T, T &, T *>				iterator;
		typedef __SListIterator<T, const T &, const T *>	const_iterator;
		typedef __SListIterator<T, Ref, Ptr>				Self;

		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef __SListNode<T> list_node;


	public:
		__SListIterator(list_node *p)
			: __SlistIteratorBase(p)
		{}
		__SListIterator()
			: __SlistIteratorBase(0)
		{}
		__SListIterator(const iterator &rhs)
			: __SlistIteratorBase(rhs.m_pNode)
		{}

	public:
		reference operator*() const
		{
			return reinterpret_cast<list_node *>(m_pNode)->m_data;
		}
		pointer operator->() const
		{
			return &(operator*());
		}

		Self &operator++()
		{
			Increment();

			return *this;
		}
		Self operator++(int)
		{
			Self tmp = *this;
			Increment();

			return tmp;
		}

		// 没有实现operator--，以为是一个forward iterator
	};


	// 单链表的大小
	inline size_t __SListSize(__SListNodeBase *pNode)
	{
		size_t count = 0;

		for(; pNode != NULL; pNode = pNode->m_pNext)
			++count;

		return count;
	}


	// 单向链表
	template<typename T, typename Alloc = Allocator<T> >
	class SList
	{
	public:
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;

		typedef __SListIterator<T, T &, T *> iterator;
		typedef __SListIterator<T, const T &, const T *> const_iterator;

	private:
		typedef __SListNode<T> list_node;
		typedef __SListNodeBase list_node_base;
		typedef __SlistIteratorBase list_iterator_base;
		
	private:
		// 头部，不是指针
		list_node_base m_head;

	public:
		SList()
		{
			m_head.m_pNext = NULL;
		}	
		SList(size_type n, const value_type &rhs)
		{
			_FillInitialize(n, rhs);
		}
		SList(int n, const value_type &rhs)
		{
			_FillInitialize(n, rhs);
		}
		SList(long n, const value_type &rhs)
		{
			_FillInitialize(n, rhs);
		}
		explicit SList(size_type n)
		{
			_FillInitialize(n, value_type());
		}
		template<typename InputIterator>
		SList(InputIterator pFirst, InputIterator pLast)
		{
			_RangeInitialize(pFirst, pLast);
		}
		SList(const SList &rhs)
		{
			_RangeInitialize(rhs.begin(), rhs.end());
		}
		template<typename T, typename Alloc>
		SList<T, Alloc> &operator=(const SList<T, Alloc> &rhs)
		{
			if( &rhs != this )
			{
				list_node_base *p1 = &m_head;
				list_node *n1 = reinterpret_cast<list_node *>(m_head.m_pNext);
				const list_node *n2 = const_cast<list_node *>(rhs.m_head.m_pNext);

				while( n1 && n2 )
				{
					n1->m_data = n2->m_data;
					p1 = n1;
					n1 = reinterpret_cast<list_node *>(n1->m_pNext);
					n2 = const_cast<list_node *>(n2->m_pNext);
				}				

				if( n2 == NULL )
					EraseAfter(p1, 0);
				else
					_InsertAfterRange(p1, const_iterator(reinterpret_cast<list_node *>(n2)), const_iterator(0));
			}


			return *this;
		}

		~SList()
		{
			clear();
		}


	public:
		iterator begin()
		{
			return iterator(reinterpret_cast<list_node *>(m_head.m_pNext));
		}
		const_iterator begin() const
		{
			return iterator(reinterpret_cast<list_node *>(m_head.m_pNext));
		}
		iterator end()
		{
			return iterator(0);
		}
		const_iterator end() const
		{
			return iterator(0);
		}

		size_type size() const
		{
			return __SListSize(m_head.m_pNext);
		}
		size_type MaxSize() const
		{
			return size_type(-1);
		}

		bool empty() const
		{
			return m_head.m_pNext == NULL;
		}

		// 只需要交换head即可
		void swap(SList &rhs)
		{
			list_iterator_base *temp	= m_head.m_pNext;
			m_head.m_pNext				= rhs.m_head.m_pNext;
			rhs.m_head.m_pNext			= temp;
		}

		// 取头部元素
		reference front() 
		{
			return reinterpret_cast<list_node *>(m_head.m_pNext)->m_data;
		}
		const_reference front() const
		{
			return reinterpret_cast<list_node *>(m_head.m_pNext)->m_data;
		}

		// 从头部插入新元素, 没有push_back
		void PushFront(const value_type &x)
		{
			__SListMakeLink(&m_head, _CreateNode(x));
		}

		// 从头部取走元素
		void PopFront()
		{
			list_node *pNode = reinterpret_cast<list_node *>(m_head.m_pNext);
			m_head.m_pNext = pNode->m_pNext;

			_DestroyNode(pNode);
		}

		iterator Prev(const_iterator pos)
		{
			return iterator(reinterpret_cast<list_node *>(__SListPrev(&m_head, pos.m_pNode)));
		}
		const_iterator Prev(const_iterator pos) const
		{
			return iterator(reinterpret_cast<list_node *>(__SListPrev(&m_head, pos.m_pNode)));
		}

		iterator InsertAfter(iterator pos, const value_type &x)
		{
			return iterator(_InsertAfter(pos.m_pNode, x));
		}
		iterator InsertAfter(iterator pos, size_type n, const value_type &x)
		{
			return _InsertAfterFill(pos.m_pNode, n, x);
		}
		template<typename InputIterator>
		iterator InsertAfter(iterator pos, InputIterator pFirst, InputIterator pLast)
		{
			return _InsertAfterRange(pos.m_pNode, pFirst, pLast);
		}

		iterator insert(iterator pos, const value_type &x)
		{
			return iterator(_InsertAfter(__SListPrev(&m_head, pos.m_pNode), x));
		}
		iterator insert(iterator pos)
		{
			return iterator(_InsertAfter(__SListPrev(&m_head, pos.m_pNode), value_type()));
		}
		void insert(iterator pos, size_type n, const value_type &x)
		{
			_InsertAfterFill(__SListPrev(&m_head, pos.m_pNode), n, x);
		}
		template<typename InputIterator>
		void insert(iterator pos, InputIterator pFirst, InputIterator pLast)
		{
			_InsertAfterRange(__SListPrev(&m_head, pos.m_pNode), pFirst, pLast);
		}

		iterator EraseAfter(iterator pos)
		{
			return iterator(reinterpret_cast<list_node *>(_EraseAfter(pos.m_pNode)));
		}
		iterator EraseAfter(iterator pFirst, iterator pLast)
		{
			return iterator(reinterpret_cast<list_node *>(_EraseAfter(pFirst.m_pNode, pLast.m_pNode)));
		}

		iterator erase(iterator pos)
		{
			return iterator(reinterpret_cast<list_node *>(_EraseAfter(__SListPrev(&m_head, pos.m_pNode))));
		}
		iterator erase(iterator pFirst, iterator pLast)
		{
			return iterator(reinterpret_cast<list_node *>(_EraseAfter(__SListPrev(&m_head, pFirst.m_pNode), pLast.m_pNode)));
		}

			
		void resize(size_type newSize, const T &x)
		{

		}
		void resize(size_type newSize)
		{
			resize(newSize, T());
		}

		void clear()
		{
			_EraseAfter(&m_head, 0);
		}

		void SpliceAfter(iterator pos, iterator pFirst, iterator pLast)
		{

		}
		void SpliceAfxter(iterator pos, iterator prev)
		{

		}

		void Splice(iterator pos, SList &rhs)
		{
		}
		void Splice(iterator pos, SList &rhs, iterator index)
		{

		}


		void Reverse()
		{
			if( m_head.m_pNext != NULL )
			{
				m_head.m_pNext = __SListReverse(m_head.m_pNext);
			}
		}

		void Remove(const T &value)
		{

		}

		void Unique()
		{

		}

		void Merge(SList &rhs)
		{

		}

		void Sort()
		{

		}

		template<typename Predicate>
		void RemoveIf(Predicate pred)
		{

		}
		template<typename BinaryPredicate>
		void Unique(BinaryPredicate pred)
		{

		}
		





	private:
		static list_node *_CreateNode(const value_type &rhs)
		{
			list_node *pNode = reinterpret_cast<list_node *>(Alloc::Allocate(sizeof(list_node)));

			try
			{
				__Construct(&pNode->m_data, rhs);
				pNode->m_pNext = NULL;
			}
			catch(...)
			{
				Alloc::Deallocate(reinterpret_cast<T *>(pNode), sizeof(list_node));
			}

			return pNode;
		}

		static void _DestroyNode(list_node *pNode)
		{
			__Destroy(&pNode->m_data);
			Alloc::Deallocate(reinterpret_cast<T *>(pNode), sizeof(list_node));
		}

		void _FillInitialize(size_type n, const value_type &rhs)
		{
			m_head.m_pNext = NULL;

			try
			{
				_InsertAfterFill(&m_head, n, rhs);
			}
			catch(...)
			{
				clear();
			}
		}


		template<typename InputIterator>
		void _RangeInitialize(InputIterator pFirst, InputIterator pLast)
		{
			m_head.m_pNext = NULL;

			try
			{
				_InsertAfterRange(&m_head, pFirst, pLast);
			}
			catch(...)
			{
				clear();
			}
		}

		void _InsertAfter(list_node_base *pos, const value_type &x)
		{
			return reinterpret_cast<list_node *>(__SListMakeLink(pos, _CreateNode(x)));
		}
		void _InsertAfterFill(list_node_base *pos, const value_type &x)
		{
			return reinterpret_cast<list_node *>(__SListMakeLink(pos, _CreateNode(x)));
		}

		template<typename InputIterator>
		void _InsertAfterRange(list_node_base *pos, InputIterator pFirst, InputIterator pLast)
		{
			while( pFirst != pLast )
			{
				pos = __SListMakeLink(pos, _CreateNode(*pFirst));
				++pFirst;
			}
		}

		// 删除pos的下一个位置的元素，返回新的下一个元素
		list_node_base *_EraseAfter(list_node_base *pos)
		{
			// 下一个元素
			list_node *pNext			= reinterpret_cast<list_node *>(pos->m_pNext);
			// 下下个元素
			list_node_base *pNextNext	= pNext->m_pNext;
			pos->m_pNext				= pNextNext;
			_DestroyNode(pNext);

			return pNextNext;
		}
		list_node_base *_EraseAfter(list_node_base *pFirst, list_node_base *pLast)
		{
			list_node *pCur = reinterpret_cast<list_node *>(pFirst->m_pNext);

			while( pCur != pLast )
			{
				list_node *tmp = reinterpret_cast<list_node *>(pCur->m_pNext);
				pCur = reinterpret_cast<list_node *>(pCur->m_pNext);
				_DestroyNode(tmp);
			}

			pFirst->m_pNext = pLast;

			return pLast;
		}
	};

}


#endif
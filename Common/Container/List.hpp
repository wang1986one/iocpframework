#ifndef __LIST_HPP
#define __LIST_HPP





namespace DataStructure
{
	// 链表节点结构，双向链表
	template<typename T>
	struct __ListNode
	{
		__ListNode<T> *m_pNext;
		__ListNode<T> *m_pPrev;
		T m_data;
	};

	// 链表迭代器, 未继承iterator
	template<typename T, typename Ref, typename Ptr>
	struct __ListIterator
	{
		typedef __ListIterator<T, Ref, Ptr>	Self;
		typedef __ListIterator<T, T&, T*>	iterator;
		typedef __ListIterator<T, const T&, const T*> const_iterator;

		// 未继承iterator，所以必须自己写五个必要的迭代器类型
		typedef BidirectionalIteratorTag	iterator_category;
		typedef T							value_type;
		typedef Ptr							pointer;
		typedef Ref							reference;
		typedef ptrdiff_t					difference_type;
		typedef __ListNode<T>*				link_type;
		typedef size_t						size_type;


	public:
		link_type m_node;

	public:
		__ListIterator()
		{}
		__ListIterator(link_type rhs)
			: m_node(rhs)
		{}
		__ListIterator(const iterator &rhs)
			: m_node(rhs.m_node)
		{}
		Self operator=(const Self &rhs)
		{
			if( this != &rhs )
			{
				m_node = rhs.m_node;
			}

			return *this;
		}
		
	public:
		// 迭代器必要的操作
		bool operator==(const Self &rhs)
		{
			return m_node == rhs.m_node;
		}
		bool operator!=(const Self &rhs)
		{
			return m_node != rhs.m_node;
		}

		// 迭代器取值(dereference)，取的是节点的值
		reference operator*() const
		{
			return (*m_node).m_data;
		}

		// 迭代器累加
		Self &operator++()
		{
			m_node = reinterpret_cast<link_type>((*m_node).m_pNext);
			return *this;
		}
		Self operator++(int)
		{
			Self tmp = *this;
			++*this;
			return tmp;
		}

		// 迭代器累减
		Self &operator--()
		{
			m_node = reinterpret_cast<link_type>((*m_node).m_pPrev);
			return *this;
		}
		Self operator--(int)
		{
			Self tmp = *this;
			--*this;

			return tmp;
		}
	};





	template<typename T, typename Alloc = Allocator<T> >
	class List
	{
	private:
		typedef void *void_pointer;
		typedef __ListNode<T> list_node;

	public:
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef list_node*			link_type;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;

		typedef __ListIterator<T, T&, T*>				iterator;
		typedef __ListIterator<T, const T&, const T*>	const_iterator;

		typedef ReverseIterator<iterator>				ReverseIterator;
		//typedef ReverseIterator<const_iterator>			ConstReverseIterator;

	private:
		link_type m_node;

		// ctor && dtor
	public:
		List()
		{
			_EmptyInitialize();	// 产生一个空节点
		}
		template<typename InputIterator>
		List(InputIterator pFirst, InputIterator pLast)
		{
			_RangeInitialize(pFirst, pLast);
		}
		List(const List<T, Alloc> &rhs)
		{
			_RangeInitialize(rhs.begin(), rhs.end());
		}
		~List()
		{
			clear();
			_PutNode(m_node);
		}	
		List<T, Alloc> &operator=(const List<T, Alloc> &rhs)
		{
			if( this != &rhs )
			{
				iterator first1 = begin();
				iterator last1  = end();
				const_iterator first2 = rhs.begin();
				const_iterator last2  = rhs.end();
				
				while(first1 != last1 && first2 != last2)
					*first1++ = *first2++;

				if( first2 == last2 )
					erase(first1, last1);
				else 
					insert(last1, first2, last2);
			}

			return *this;
		}


	public:
		iterator begin()
		{
			return reinterpret_cast<link_type>((*m_node).m_pNext);
		}
		const_iterator begin() const
		{
			return reinterpret_cast<link_type>((*m_node).m_pNext);
		}

		iterator end()
		{
			return m_node;
		}
		const_iterator end() const
		{
			return m_node;
		}

		ReverseIterator RBegin()
		{
			return ReverseIterator(end());
		}
		/*ConstReverseIterator RBegin() const
		{
			return ConstReverseIterator(end());
		}*/

		ReverseIterator REnd()
		{
			return ReverseIterator(begin());
		}
		/*ConstReverseIterator REnd() const
		{
			return ConstReverseIterator(begin());
		}*/

		bool empty() const
		{
			return m_node->m_pNext == m_node;
		}
		size_type size() const
		{
			size_type res = 0;
			DistanceFunc(begin(), end(), res);

			return res;
		}

		size_type MaxSize() const
		{
			return size_type(-1);
		}

		// 取节点头的值
		reference front()
		{
			return *begin();
		}
		const_reference front() const
		{
			return *begin();
		}

		// 取尾节点的值
		reference back()
		{
			return *(--end());
		}
		const_reference back() const
		{
			return *(--end());
		}

		void swap(List<T, Alloc> &rhs)
		{
			swap(m_node, rhs.m_node);
		}

		// 在迭代器指定位置
		iterator insert(iterator pos, const T &x)
		{
			link_type tmp = _CreateNode(x);

			tmp->m_pNext = pos.m_node;
			tmp->m_pPrev = pos.m_node->m_pPrev;

			reinterpret_cast<link_type>(pos.m_node->m_pPrev)->m_pNext = tmp;
			pos.m_node->m_pPrev = tmp;

			return tmp;
		}
		iterator insert(iterator pos)
		{
			return insert(pos, T());
		}
		template<typename InputIterator>
		void insert(iterator pos, InputIterator pFirst, InputIterator pLast)
		{
			for(; pFirst != pLast; ++pFirst)
				insert(pos, *pFirst);
		}
		void insert(iterator pos, size_type n, const T &x)
		{
			for(; n > 0; --n)
				insert(pos, x);
		}
		void insert(iterator pos, int n, const T &x)
		{
			return insert(pos, static_cast<size_type>(n), x);
		}
		void insert(iterator pos, long n, const T &x)
		{
			return insert(pos, static_cast<size_type>(n), x);
		}


		// 插入一个节点，作为头结点
		void PushFront(const T &x)
		{
			insert(begin(), x);
		}
		// 插入一个节点，作为尾节点
		void push_back(const T &x)
		{
			insert(end(), x);
		}


		// 移除迭代器所指定的节点
		iterator erase(iterator pos)
		{
			link_type pNextNode = reinterpret_cast<link_type>(pos.m_node->m_pNext);
			link_type pPrevNode = reinterpret_cast<link_type>(pos.m_node->m_pPrev);

			pPrevNode->m_pNext = pNextNode;
			pNextNode->m_pPrev = pPrevNode;

			_DestroyNode(pos.m_node);

			return pNextNode;
		}
		iterator erase(iterator pFirst, iterator pLast)
		{
			while(pFirst != pLast)
				erase(pFirst++);

			return pLast;
		}

		// 移除头结点
		void PopFront()
		{
			erase(begin());
		}
		// 移除尾节点
		void pop_back()
		{
			iterator tmp = end();
			erase(--tmp);
		}

		void resize(size_type newSize, const T &x)
		{
			iterator iter = begin();
			size_type len = 0;

			for(; iter != len && len < newSize; ++iter, ++len)
				;

			if( len == newSize )
				erase(iter, end());
			else						//  iter == end()
				insert(end(), newSize - len, x);
		}
		void resize(size_type newSize)
		{
			resize(newSize, T());
		}

		void clear()
		{
			link_type cur = reinterpret_cast<link_type>(m_node->m_pNext);	// begin()
			while( cur != m_node )
			{
				link_type tmp = cur;
				cur = reinterpret_cast<link_type>(cur->m_pNext);
				_DestroyNode(tmp);
			}

			// 恢复m_node的原始状态
			m_node->m_pNext = m_node;
			m_node->m_pPrev = m_node;
		}

		// 结合于pos所指定位置之前
		void Splice(iterator pos, List &rhs)
		{
			if( !rhs.empty() )
				_Transfer(pos, rhs.begin(), rhs.end());
		}
		// 将所指元素结合于pos位置之前
		void Splice(iterator pos, List &rhs, iterator iter)
		{
			iterator tmp = iter;
			++tmp;

			if( pos == iter || pos == tmp )
				return;

			_Transfer(pos, iter, tmp);
		}
		// 将[first, last)内的所有元素结合于pos之前
		void Splice(iterator pos, List &, iterator pFirst, iterator pLast)
		{
			if( pFirst != pLast )
				_Transfer(pos, pFirst, pLast);
		}

		void Remove(const T &val)
		{
			iterator first = begin();
			iterator last  = end();

			while( first != last )
			{
				iterator iter = first;
				++iter;

				if( *first == val )
					erase(first);

				first = iter;
			}
		}

		// 移除数值相同的连续元素
		void Unique()
		{
			iterator first = begin();
			iterator last  = end();

			if( first == last )
				return;

			iterator iter = first;

			while( ++iter != last )
			{
				if( *first == *iter )
					erase(iter);
				else
					first = iter;

				iter = first;
			}
		}

		// 合并两个List
		void Merge(List<T, Alloc> &rhs)
		{
			iterator first1 = begin();
			iterator last1  = end();
			iterator first2 = rhs.begin();
			iterator last2  = rhs.end();

			// 两个List必须排好序
			while(first1 != last1 && first2 != last2)
			{
				if( *first2 < *first1 )
				{
					iterator iter = first2;
					_Transfer(first1, first2, ++iter);
					first2 = iter;
				}
				else
					++first1;
			}

			if( first2 != last2 )
				_Transfer(last1, first2, last2);
		}

		// 翻转List内所有元素
		void Reverse()
		{
			// 如果是空列表或者只有一个元素，就不做任何翻转
			if( m_node->m_pNext == m_node || reinterpret_cast<link_type>(m_node->m_pNext)->m_pNext == m_node )
				return;

			iterator first = begin();
			++first;

			while(first != end())
			{
				iterator oldIter = first;
				++first;

				_Transfer(begin(), oldIter, first);
			}
		}

		template<typename Predicate>
		void RemoveIf(Predicate pred)
		{
			iterator first = begin();
			iterator last = end();

			while(first != last)
			{
				iterator iter = first;
				++iter;

				if( pred(*first) )
					erase(first);

				first = iter;
			}
		}

		template<typename BinaryPredicate>
		void Unique(BinaryPredicate binaryPred)
		{
			iterator first = begin();
			iterator last = end();

			if( first == last )
				return;

			iterator iter = first;
			
			while(++iter != last)
			{
				if( binaryPred(*first, *iter) )
					erase(iter);
				else
					first = iter;

				iter = first;
			}
		}	

		template<typename StrictWeakOrdering>
		void Merge(List<T, Alloc> &rhs, StrictWeakOrdering cmp)
		{
			iterator first1 = begin();
			iterator last1	= end();
			iterator first2 = rhs.begin();
			iterator last2	= rhs.end();

			while(first1 != last1 && first2 != last2)
				if( cmp(*first2, *first1) )
				{
					iterator next = first2;
					_Transfer(first1, first2, ++next);
					first2 = next;
				}
				else
					++first1;

			if( first2 != last2 ) 
				_Transfer(last1, first2, last2);

		}

		// List 不能使用STL通用版的Sort算法，因为STL的Sort算法是基于RamdonAccessIterator
		// 本算法采用Quick Sort
		void Sort()
		{
			// 判断是否为空链表，或者仅有一个元素
			// 也可以使用size() == 0 || size() == 1进行判断
			if( m_node->m_pNext == m_node || reinterpret_cast<link_type>(m_node->m_pNext)->m_pNext == m_node )
				return;

			// 一些新的Lists,做缓冲区
			List<T, Alloc> buf;
			List<T, Alloc> counter[64];

			int nFill = 0;
			while(!empty())
			{
				buf.Splice(buf.begin(), *this, begin());

				int nIndex = 0;
				while( nIndex < nFill && !counter[nIndex].empty() )
				{
					counter[nIndex].Merge(buf);
					buf.swap(counter[nIndex++]);
				}

				buf.swap(counter[nIndex]);
				if( nIndex == nFill )
					++nFill;
			}

			for(int i = 1; i < nFill; ++i)
			{
				counter[i].Merge(counter[i - 1]);
			}

			swap(counter[nFill -1]);
		}

		friend bool operator==(const List<T, Alloc> &lhs, const List<T, Alloc> &rhs)
		{
			link_type e1 = lhs.m_node;
			link_type e2 = rhs.m_node;

			link_type n1 = reinterpret_cast<link_type>(e1->m_pNext);
			link_type n2 = reinterpret_cast<link_type>(e2->m_pPrev);

			for(; n1 != e1 && n2 != e2; 
				n1 = reinterpret_cast<link_type>(n1->m_pNext), n2 = reinterpret_cast<link_type>(n2->m_pNext))
			{
				if( n1->m_data != n2->m_data )
					return false;
			}

			return n1 == e2 && n2 = e2;
		}

	private:
		// 返回一个节点
		link_type _GetNode()
		{
			return reinterpret_cast<link_type>(Alloc::Allocate(sizeof(list_node)));
		}
		// 释放一个节点
		void _PutNode(link_type p)
		{
			return Alloc::Deallocate(reinterpret_cast<pointer>(p), sizeof(list_node));
		}
		// 产生一个节点，带有元素值
		link_type _CreateNode(const T &x)
		{
			link_type p = _GetNode();

			try
			{
				__Construct(&p->m_data, x);
			}
			catch(...)
			{
				_PutNode(p);
			}

			return p;
		}
		// 销毁一个节点
		void _DestroyNode(link_type p)
		{
			__Destroy(&p->m_data);
			_PutNode(p);
		}

		void _EmptyInitialize()
		{
			m_node = _GetNode();		// 配置一个节点空间，令m_node指向自己
			m_node->m_pNext = m_node;
			m_node->m_pPrev = m_node;
		}
		void _FillInitialize(size_type n, const T &val)
		{
			_EmptyInitialize();

			try
			{
				insert(begin(), n, val);
			}
			catch(...)
			{
				clear();
				_PutNode(m_node);
			}
		}

		template<typename Inputiterator>
		void _RangeInitialize(Inputiterator pFirst, Inputiterator pLast)
		{
			_EmptyInitialize();

			try
			{
				insert(begin(), pFirst, pLast);
			}
			catch(...)
			{
				clear();
				_PutNode(m_node);
			}
		}
		void _RangeInitialize(const_iterator pFirst, const_iterator pLast)
		{
			_EmptyInitialize();

			try
			{
				insert(begin(), pFirst, pLast);
			}
			catch(...)
			{
				clear();
				_PutNode(m_node);
			}
		}
		
		// 将[first, last)内的所有元素搬移到position处
		void _Transfer(iterator pos, iterator pFirst, iterator pLast)
		{
			if( pos != pLast )
			{
				*(reinterpret_cast<link_type>((*pLast.m_node).m_pPrev)).m_pNext	= pos.m_node;
				*(reinterpret_cast<link_type>((*pFirst.m_node).m_pPrev)).m_pNext = pLast.m_node;
				*(reinterpret_cast<link_type>((*pos.m_node).m_pPrev)).m_pNext	= pos.m_node;

				link_type tmp = reinterpret_cast<link_type>((*pos.m_node).m_pPrev);

				(*pos.m_node).m_pPrev		= (*pLast.m_node).m_pPrev;
				(*pLast.m_node).m_pPrev		= (*pFirst.m_node).m_pPrev;
				(*pFirst.m_node).m_pPrev	= tmp;
			}
		}


	};
}


#endif
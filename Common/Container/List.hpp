#ifndef __LIST_HPP
#define __LIST_HPP





namespace DataStructure
{
	// ����ڵ�ṹ��˫������
	template<typename T>
	struct __ListNode
	{
		__ListNode<T> *m_pNext;
		__ListNode<T> *m_pPrev;
		T m_data;
	};

	// ���������, δ�̳�iterator
	template<typename T, typename Ref, typename Ptr>
	struct __ListIterator
	{
		typedef __ListIterator<T, Ref, Ptr>	Self;
		typedef __ListIterator<T, T&, T*>	iterator;
		typedef __ListIterator<T, const T&, const T*> const_iterator;

		// δ�̳�iterator�����Ա����Լ�д�����Ҫ�ĵ���������
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
		// ��������Ҫ�Ĳ���
		bool operator==(const Self &rhs)
		{
			return m_node == rhs.m_node;
		}
		bool operator!=(const Self &rhs)
		{
			return m_node != rhs.m_node;
		}

		// ������ȡֵ(dereference)��ȡ���ǽڵ��ֵ
		reference operator*() const
		{
			return (*m_node).m_data;
		}

		// �������ۼ�
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

		// �������ۼ�
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
			_EmptyInitialize();	// ����һ���սڵ�
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

		// ȡ�ڵ�ͷ��ֵ
		reference front()
		{
			return *begin();
		}
		const_reference front() const
		{
			return *begin();
		}

		// ȡβ�ڵ��ֵ
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

		// �ڵ�����ָ��λ��
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


		// ����һ���ڵ㣬��Ϊͷ���
		void PushFront(const T &x)
		{
			insert(begin(), x);
		}
		// ����һ���ڵ㣬��Ϊβ�ڵ�
		void push_back(const T &x)
		{
			insert(end(), x);
		}


		// �Ƴ���������ָ���Ľڵ�
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

		// �Ƴ�ͷ���
		void PopFront()
		{
			erase(begin());
		}
		// �Ƴ�β�ڵ�
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

			// �ָ�m_node��ԭʼ״̬
			m_node->m_pNext = m_node;
			m_node->m_pPrev = m_node;
		}

		// �����pos��ָ��λ��֮ǰ
		void Splice(iterator pos, List &rhs)
		{
			if( !rhs.empty() )
				_Transfer(pos, rhs.begin(), rhs.end());
		}
		// ����ָԪ�ؽ����posλ��֮ǰ
		void Splice(iterator pos, List &rhs, iterator iter)
		{
			iterator tmp = iter;
			++tmp;

			if( pos == iter || pos == tmp )
				return;

			_Transfer(pos, iter, tmp);
		}
		// ��[first, last)�ڵ�����Ԫ�ؽ����pos֮ǰ
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

		// �Ƴ���ֵ��ͬ������Ԫ��
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

		// �ϲ�����List
		void Merge(List<T, Alloc> &rhs)
		{
			iterator first1 = begin();
			iterator last1  = end();
			iterator first2 = rhs.begin();
			iterator last2  = rhs.end();

			// ����List�����ź���
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

		// ��תList������Ԫ��
		void Reverse()
		{
			// ����ǿ��б����ֻ��һ��Ԫ�أ��Ͳ����κη�ת
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

		// List ����ʹ��STLͨ�ð��Sort�㷨����ΪSTL��Sort�㷨�ǻ���RamdonAccessIterator
		// ���㷨����Quick Sort
		void Sort()
		{
			// �ж��Ƿ�Ϊ���������߽���һ��Ԫ��
			// Ҳ����ʹ��size() == 0 || size() == 1�����ж�
			if( m_node->m_pNext == m_node || reinterpret_cast<link_type>(m_node->m_pNext)->m_pNext == m_node )
				return;

			// һЩ�µ�Lists,��������
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
		// ����һ���ڵ�
		link_type _GetNode()
		{
			return reinterpret_cast<link_type>(Alloc::Allocate(sizeof(list_node)));
		}
		// �ͷ�һ���ڵ�
		void _PutNode(link_type p)
		{
			return Alloc::Deallocate(reinterpret_cast<pointer>(p), sizeof(list_node));
		}
		// ����һ���ڵ㣬����Ԫ��ֵ
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
		// ����һ���ڵ�
		void _DestroyNode(link_type p)
		{
			__Destroy(&p->m_data);
			_PutNode(p);
		}

		void _EmptyInitialize()
		{
			m_node = _GetNode();		// ����һ���ڵ�ռ䣬��m_nodeָ���Լ�
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
		
		// ��[first, last)�ڵ�����Ԫ�ذ��Ƶ�position��
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
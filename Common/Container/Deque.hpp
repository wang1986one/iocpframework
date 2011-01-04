#ifndef __DEQUE_HPP
#define __DEQUE_HPP



namespace DataStructure
{
	template<typename T, typename Ref, typename Ptr, size_t BufSize>
	struct __DequeIterator
	{
		typedef __DequeIterator<T, T&, T*, BufSize>				iterator;
		typedef __DequeIterator<T, const T&, const T*, BufSize> const_iterator;
		typedef RandomAccessIteratorTag							iterator_category;
		typedef T												value_type;
		typedef Ptr												pointer;
		typedef Ref												reference;
		typedef size_t											size_type;
		typedef ptrdiff_t										difference_type;
		typedef T**												map_pointer;
		typedef __DequeIterator									Self;


		T *m_pCur;				// 迭代器所指当前元素
		T *m_pFirst;			// 迭代器所指之前区域的头
		T *m_pLast;				// 迭代器所指之后区域的尾
		map_pointer m_pNode;

		
		__DequeIterator(T *x, map_pointer ptr)
			: m_pCur(x)
			, m_pFirst(*ptr)
			, m_pLast(*ptr + BufferSize())
			, m_pNode(ptr)
		{}
		__DequeIterator()
			: m_pCur(0)
			, m_pFirst(0)
			, m_pLast(0)
			, m_pNode(0)
		{}
		__DequeIterator(const iterator &x)
			: m_pCur(x)
			, m_pFirst(x.m_pFirst)
			, m_pLast(x.m_pLast)
			, m_pNode(x.m_pNode)
		{}

		// 重载各种所需操作符
		reference operator*() const
		{
			return *m_pCur;
		}
		pointer operator->() const
		{
			return &(operator*());
		}

		difference_type operator-(const Self &rhs) const
		{
			return difference_type(BufferSize()) * (m_pNode - rhs.m_pNode - 1) + 
				(m_pCur - m_pFirst) + (rhs.m_pLast - rhs.m_pCur);
		}

		Self &operator++()
		{
			++m_pCur;						// 切换至下一个元素
			if( m_pCur == m_pLast )			// 如果已达所在缓冲区的尾端
			{	
				_SetNode(m_pNode + 1);		// 切换至下一个节点的第一个元素
				m_pCur = m_pFirst;
			}

			return *this;
		}

		Self operator++(int)
		{
			Self tmp = *this;
			++*this;

			return tmp;
		}

		Self &operator--()
		{
			if( m_pCur == m_pFirst )			// 如果已达所在缓冲区的头端
			{	
				_SetNode(m_pNode - 1);			// 切换至上个节点的最后一个元素
				m_pCur = m_pLast;
			}

			--m_pCur;							// 切换至前一个元素
			return *this;
		}

		// 实现随机存取，迭代器可以跳跃n个距离
		Self &operator+=(difference_type n)
		{
			difference_type offset = n + (m_pCur - m_pFirst);

			// 目标位置在同一缓冲区内
			if( offset >= 0 && offset < difference_type(BufferSize()) )
				m_pCur += n;
			// 不在同一缓冲区内
			else
			{
				difference_type nodeOffset = offset > 0 ? offset / difference_type(BufferSize()) :
					-difference_type((-offset - 1) / BufferSize()) - 1;
				
				// 切换至正确节点
				_SetNode(m_pNode + nodeOffset);
				// 切换至正确的元素
				m_pCur = m_pFirst + (offset - nodeOffset * difference_type(BufferSize()));
			}
		}

		Self operator+(difference_type n) const
		{
			Self tmp = *this;
			return tmp += n;	// 调用operator+=
		}

		Self &operator-=(difference_type n)
		{
			return *this += -n;
		}

		Self operator-(difference_type n) const
		{
			Self tmp = *this;
			return tmp -= n;	// 调用operator-=
		}

		reference operator[](difference_type n) const
		{
			return *(*this + n);	// 调用operator* operator+
		}

		bool operator==(const Self &x) const
		{
			return m_pCur == x.m_pCur;
		}

		bool operator!=(const Self &x) const
		{
			return !(*this == x);
		}

		bool operator<(const Self &x) const
		{
			return (m_pNode == x.m_pNode) ? (m_pCur < x.m_pCur) : (m_pNode < x.m_pNode);
		}

		static size_t _BufferSize() 
		{
			return BufSize != 0 ? BufSize : (BufSize < 512 ? size_t(512 / sizeof(T)) : size_t(1));
		}

	private:
		void _SetNode(map_pointer newNode)
		{
			m_pNode		= newNode;
			m_pFirst	= *newNode;
			m_pLast		= m_pFirst + difference_type(_BufferSize());
		}
	};



	template<typename T, typename Alloc = Allocator<T>, size_t BufSize = 0>
	class Deque
	{
	public:
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;

		typedef __DequeIterator<T, T&, T*, BufSize>              iterator;
		typedef __DequeIterator<T, const T&, const T&, BufSize>  const_iterator;
	
		typedef ReverseIterator<const_iterator>					ConstReverseIterator;
		typedef ReverseIterator<iterator>						ReverseIterator;

		typedef pointer*			map_pointer;

	private:
		iterator m_pStart;		// 第一个节点
		iterator m_pFinish;		// 最后一个节点
		map_pointer m_pMap;		// 指向map,map是块连续空间
		size_type m_nMapSize;	// map内有多少指针


	public:
		Deque()
			: m_pStart()
			, m_pFinish()
			, m_pMap(0)
			, m_nMapSize(0)
		{
			_CreateMapAndNodes(0);
		}
		Deque(const Deque& x)
			: m_pStart()
			, m_pFinish()
			, m_pMap(0)
			, m_nMapSize(0)
		{
			_CreateMapAndNodes(x.size());

			try 
			{
				__UninitializedCopy(x.begin(), x.end(), m_pStart);
			}
			catch(...)
			{
				_DestroyMapAndNodes();
			}
		}
		Deque(size_type n, const value_type& value)
			: m_pStart()
			, m_pFinish()
			, m_pMap(0)
			, m_nMapSize(0)
		{
			_FillInitialize(n, value);
		}
		Deque(int n, const value_type& value)
			: m_pStart()
			, m_pFinish()
			, m_pMap(0)
			, m_nMapSize(0)
		{
			_FillInitialize(n, value);
		}
		Deque(long n, const value_type& value)
			: m_pStart()
			, m_pFinish()
			, m_pMap(0)
			, m_nMapSize(0)
		{
			_FillInitialize(n, value);
		}
		explicit Deque(size_type n)
			: m_pStart()
			, m_pFinish()
			, m_pMap(0)
			, m_nMapSize(0)
		{
			_FillInitialize(n, value_type());
		}
		template<typename InputIterator>
		Deque(InputIterator first, InputIterator last)
			: m_pStart()
			, m_pFinish()
			, m_pMap(0)
			, m_nMapSize(0)
		{
			_RangeInitialize(first, last, IteratorTraits<InputIterator>::iterator_category());
		}

		~Deque() 
		{
			__Destroy(m_pStart, m_pFinish);
			_DestroyMapAndNodes();
		}

		Deque& operator= (const Deque& rhs) 
		{
			const size_type len = size();

			if( &rhs != this )
			{
				if( len >= rhs.size() )
					erase(copy(rhs.begin(), rhs.end(), m_pStart), m_pFinish);
				else 
				{
					const_iterator mid = rhs.begin() + difference_type(len);
					copy(rhs.begin(), mid, m_pStart);
					insert(m_pFinish, mid, rhs.end());
				}
			}

			return *this;
		}  


	public:                        
		iterator begin() 
		{ 
			return m_pStart; 
		}
		const_iterator begin() const 
		{ 
			return m_pStart; 
		}

		iterator end() 
		{
			return m_pFinish; 
		}
		const_iterator end() const 
		{ 
			return m_pFinish; 
		}

		ReverseIterator RBegin() 
		{
			return ReverseIterator(m_pFinish); 
		}
		ConstReverseIterator RBegin() const 
		{
			return ConstReverseIterator(m_pFinish);
		}

		ReverseIterator REnd()
		{ 
			return ReverseIterator(m_pStart); 
		}
		ConstReverseIterator REnd() const 
		{
			return ConstReverseIterator(m_pStart);
		}

		reference operator[](size_type n) 
		{
			return m_pStart[difference_type(n)]; 
		}
		const_reference operator[](size_type n) const
		{
			return m_pStart[difference_type(n)];
		}

		reference front() 
		{
			return *m_pStart; 
		} 
		const_reference front() const 
		{
			return *m_pStart; 
		}

		reference back() 
		{
			iterator tmp = m_pFinish;	
			--tmp;	
			return *tmp; 	
		}
		const_reference back() const
		{
			const_iterator tmp = m_pFinish;
			--tmp;
			return *tmp;
		}

		size_type size() const 
		{ 
			return m_pFinish - m_pStart;
		}

		size_type MaxSize() const 
		{ 
			return size_type(-1);
		}

		bool empty() const
		{ 
			return m_pFinish == m_pStart; 
		}

		void swap(Deque<T>& rhs)
		{
			swap(m_pStart, rhs.m_pStart);
			swap(m_pFinish, rhs.m_pFinish);
			swap(m_pMap, rhs.m_pMap);
			swap(m_nMapSize, rhs.m_nMapSize);
		}


		void push_back(const value_type& t)
		{
			if( m_pFinish.m_pCur != m_pFinish.m_pLast - 1 )
			{ 
				// 最后缓冲区还有一个以上的备用空间
				__Construct(finish.cur, t);	
				// 调整
				++m_pFinish.m_pCur;			
			}
			else  // 最后已无可用空间
				PushBackAux(t);
		}

		void PushFront(const value_type& t) 
		{
			if( m_pStart.m_pCur != m_pStart.m_pFirst ) 
			{ 	
				// 第一缓冲区还有备用空间
				__Construct(m_pStart.m_pCur - 1, t); 	
				--m_pStart.m_pCur;		
			}
			else // 第一缓冲区已无可用空间
				PushFrontAux(t);
		}

		void pop_back()
		{
			if( m_pFinish.m_pCur != m_pFinish.m_pFirst )
			{
				// 最后缓冲区还有一个可用空间
				--m_pFinish.m_pCur;		
				__Destroy(m_pFinish.m_pCur);	
			}
			else
				// 最後緩衝區沒有任何元素
				PopBackAux();		// 這裡將進行緩衝區的釋放工作
		}

		void PopFront() 
		{
			if( m_pStart.m_pCur != m_pStart.m_pLast - 1 )
			{
				// 第一缓冲区有一个（或更多）元素
				__Destroy(m_pStart.m_pCur);	
				++m_pStart.m_pCur;			
			}
			else 
				// 第一缓冲区仅有一个元素
				PopFrontAux();	
		}


		// 在position插入，其值为 x
		iterator insert(iterator position, const value_type& x)
		{
			if( position.m_pCur == m_pStart.m_pCur )
			{	
				// 在deque最前端插入
				PushFront(x);		
				return m_pStart;
			}
			else if( position.m_pCur == m_pFinish.m_pCur ) 
			{ 
				// 在deque最尾端插入
				push_back(x);					
				iterator tmp = m_pFinish;
				--tmp;

				return tmp;
			}
			else 
			{
				return _InsertAux(position, x);	
			}
		}
		iterator insert(iterator position)
		{ 
			return insert(position, value_type()); 
		}
		void insert(iterator pos, size_type n, const value_type& x)
		{

		}
		template<typename InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last) 
		{
			insert(pos, first, last, IteratorTraits<InputIterator>::value_type());
		}

		void resize(size_type newSize, const value_type& x)
		{
			const size_type len = size();

			if( newSize < len ) 
				erase(m_pStart + newSize, m_pFinish);
			else
				insert(m_pFinish, newSize - len, x);
		}

		void resize(size_type newSize) 
		{ 
			resize(newSize, value_type()); 
		}

		// 清除 pos 所指的元素
		iterator erase(iterator pos)
		{
			iterator next = pos;
			++next;

			difference_type index = pos - m_pStart;	// 清除点之前的元素个数

			if( index < (size() >> 1) ) 
			{
				// 就搬移清除點之前的元素
				CopyBackward(m_pStart, pos, next);
				// 去除最前一个元素
				PopFront();				
			}
			else 
			{	
				// 就搬移清除点之后的元素
				copy(next, m_pFinish, pos);	
				// 去除最后一个元素
				pop_back();				
			}

			return m_pStart + index;
		}
		iterator erase(iterator first, iterator last)
		{

		}

		void clear()
		{

		}

		







	private:
		static size_type _BufferSize() 
		{
			return iterator::_BufferSize();
		}
		static size_type _InitialMapSize() 
		{ 
			return 8; 
		}


	private:
		void _CreateMapAndNodes(size_type num_elements)
		{
		}
		void _DestroyMapAndNodes()
		{

		}

		void _FillInitialize(size_type n, const value_type& value)
		{

		}

		template<typename InputIterator>
		void _RangeInitialize(InputIterator first, InputIterator last, InputIteratorTag)
		{

		}

		template<typename ForwardIterator>
		void _RangeInitialize(ForwardIterator first, ForwardIterator last, ForwardIteratorTag)
		{

		}

		void _PushBackAux(const value_type& t)
		{

		}

		void _PushFrontAux(const value_type& t)
		{

		}

		void PopBackAux()
		{

		}

		void PopFrontAux()
		{

		}

	};
}






#endif
#ifndef __VECTOR_HPP
#define __VECTOR_HPP

#include "../Basic.hpp"


namespace DataStructure
{
	template<typename T, typename AllocT = Allocator<T> >
	class Vector
	{
	public:
		// IteratorTraits<T>所定义的型别
		typedef	T						value_type;
		typedef value_type*				pointer;
		typedef const value_type*		const_pointer;
		typedef value_type*				iterator;
		typedef const value_type*		const_iterator;	
		typedef value_type&				reference;
		typedef const value_type&		const_reference;
		typedef size_t					size_type;
		typedef ptrdiff_t				difference_type;
		// 由于vector所维护的是一个连续性空间，所以不论元素型别，原生指针都可以作为其迭代器而满足要求

		typedef ReverseIterator<iterator>				ReverseIteratorType;
		typedef ReverseIterator<const_iterator>			ConstReverseIteratorType;
		


	private:
		// vector 采用简单的线性连续空间，以两个迭代器start和end分别指向头尾，并以迭代器end_of_storage指向容量尾端
		iterator m_pStart;
		iterator m_pEnd;
		iterator m_pEndStorage;
		
	public:
		// ctor
		Vector()
			: m_pStart(NULL)
			, m_pEnd(NULL)
			, m_pEndStorage(NULL)
		{}
		explicit Vector(size_type n)
		{
			_Initialize(n, T());
		}
		Vector(size_type n, const T &val)
		{
			_Initialize(n, val);
		}
		Vector(int n, const T &val)
		{
			_Initialize(n, val);
		}
		Vector(long n, const T &val)
		{
			_Initialize(n, val);
		}
		
		template<typename InputIteratorT>
		Vector(InputIteratorT pFirst, InputIteratorT pLast)
			: m_pStart(NULL)
			, m_pEnd(NULL)
			, m_pEndStorage(NULL)
		{
			_RangeInitialize(pFirst, pLast, IteratorTraits<InputIteratorT>::iterator_category());
		}


		// dtor
		~Vector()
		{
			__Destroy(m_pStart, m_pEnd);
			_Deallocate();
		}

		Vector<T, AllocT> &operator=(const Vector<T, AllocT> &rhs)
		{
			if( &rhs != this )
			{
				if( rhs.size() > capacity() )
				{
					// 如果目标容器体积更大
					iterator iterTemp = _AllocateAndCopy(rhs.end() - rhs.begin());

					__Destroy(m_pStart, m_pEnd);	// 销毁整个Vector
					_Deallocate();					// 释放空间

					m_pStart = iterTemp;
					m_pEndStorage = m_pStart + (rhs.end() - rhs.begin());
				}
				else if( size() >= rhs.size() )
				{
					// 如果目标容器大小小于等于本身的大小
					iterator iter = copy(rhs.begin(), rhs.end(), begin());
					__Destroy(iter, m_pEnd);
				}
				else
				{
					copy(rhs.begin(), rhs.begin() + size(), m_pStart);
					__UninitializedCopy(rhs.begin() + size(), rhs.end(), m_pEnd);
				}

				m_pEnd = m_pStart + rhs.size();
			}

			return *this;
		}

		// interface
	public:
		const_pointer data() const
		{
			return begin();
		}

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
			return m_pEnd;
		}
		const_iterator end() const
		{
			return m_pEnd;
		}
		

		size_type size() const
		{
			return size_type(end() - begin());
		}
		size_type capacity() const
		{
			return size_type(m_pEndStorage - begin());
		}

		bool empty() const
		{
			return begin() == end();
		}
		reference operator[](size_type n)
		{
			return *(begin() + n);
		}
		const_reference operator[](size_type n) const
		{
			return *(begin() + n);
		}
		

		void reserve(size_type n)
		{
			if( capacity() < n )
			{
				const size_type oldSize = size();
				iterator pTemp = _AllocateAndCopy(n, m_pStart, m_pEnd);

				__Destroy(m_pStart, m_pEnd);
				_Deallocate();

				m_pStart	= pTemp;
				m_pEnd		= pTemp + oldSize;
				m_pEndStorage = m_pStart + n;
			}	
		}


		// 取出第一个元素
		reference front()
		{
			return *begin();
		}
		const_reference front() const
		{
			return *begin();
		}
	
		// 取出最后一个元素
		reference back()
		{
			return *(end() - 1);
		}
		const_reference back() const
		{
			return *(end() - 1);
		}

		// 增加一个元素，放到最后
		void push_back(const T &x)
		{
			if( m_pEnd != m_pEndStorage )
			{
				// 还有备用空间，直接在备用空间中构建元素
				__Construct(m_pEnd, x);

				++m_pEnd;	// 调整结束位置
			}
			else
			{
				// 已无备用空间
				_InsertAux(end(), x);
			}
		}

		void pop_back()
		{
			--m_pEnd;
			__Destroy(m_pEnd);
		}


		iterator erase(iterator pos)
		{
			if( pos + 1 != end() )
			{
				// 如果不是指向最后一个元素
				// 将pos之后的元素一个一个向前移动
				copy(pos + 1, m_pEnd, pos);
			}

			--m_pEnd;
			__Destroy(m_pEnd);

			return pos;
		}
		iterator erase(iterator pFirst, iterator pEnd)
		{
			iterator iter = Copy(pEnd, m_pEnd, pFirst);
			__Destroy(iter, m_pEnd);

			m_pEnd = m_pEnd - (pEnd - pFirst);
			return pFirst;
		}

		void resize(size_type newSize, const T &x)
		{
			if( newSize < size() )
				erase(begin() + newSize, end());
			else
				insert(end(), newSize - size(), x);
		}
		void resize(size_type newSize)
		{
			resize(newSize, T());
		}
		
		void clear()
		{
			erase(begin(), end());
		}

		void swap(Vector<T, AllocT> &rhs)
		{
			Swap(m_pStart, rhs.m_pStart);
			Swap(m_pEnd, rhs.m_pEnd);
			Swap(m_pEndStorage, rhs.m_pEndStorage);
		}

		iterator insert(iterator pos, const T &x)
		{
			size_type n = pos - begin();
			if( m_pEnd != m_pEndStorage  && pos == end() )
			{
				__Construct(m_pEnd, x);
				++m_pEnd;
			}
			else
			{
				_InsertAux(pos, x);
			}

			return begin() + n;
		}
		iterator insert(iterator pos)
		{
			return insert(pos, T());
		}
		template<typename InputIterator>
		void insert(iterator pos, InputIterator pFirst, InputIterator pLast)
		{
			_RangeInsert(pos, pFirst, pLast, iterator_category(pFirst));
		}
		void insert(iterator pos, int n, const T &x)
		{
			for(int i = 0; i != n; ++i)
				insert(pos, x);
		}



	private:
		void _InsertAux(iterator pos, const T &x)
		{
			if( m_pEnd != m_pEndStorage )
			{
				// 还有备用空间
				// 在备用空间起始处建构一个元素，并以Vector最后一个元素值为初始值
				__Construct(m_pEnd, *(m_pEnd - 1));
				++m_pEnd;
			}
			else
			{		// 已無備用空間
				const size_type oldSize = size();
				const size_type len = oldSize != 0 ? 2 * oldSize : 1;
				// 以上配置原則：如果原大小為0，則配置 1（個元素大小）；
				// 如果原大小不為0，則配置原大小的兩倍，
				// 前半段用來放置原資料，後半段準備用來放置新資料。

				iterator newStart = AllocT::Allocate(len); // 實際配置
				iterator newFinish = newStart;
				
				try
				{
					// 將原vector 的內容拷貝到新 vector。
					newFinish = __UninitializedCopy(m_pStart, pos, newStart);
					// 為新元素設定初值x
					__Construct(newFinish, x);
					// 調整水位。
					++newFinish;
					// 將原vector 的備用空間中的內容也忠實拷貝過來（啥用途？）
					newFinish = __UninitializedCopy(pos, m_pEnd, newFinish);
				}
				catch(...) 
				{
					// "commit or rollback" 語意：若非全部成功，就一個不留。
					__Destroy(newStart, newFinish); 
					AllocT::Deallocate(newStart, len);
					throw;
				}

				// 解構並釋放原 vector
				__Destroy(begin(), end());
				_Deallocate();

				// 調整迭代器，指向新vector
				m_pStart		= newStart;
				m_pEnd			= newFinish;
				m_pEndStorage	= newStart + len;
			}

		}

		void _Deallocate()
		{
			if( m_pStart != NULL )
			{
				AllocT::Deallocate(m_pStart, m_pEndStorage - m_pStart);
			}
		}

		void _Initialize(size_type n, const value_type &val)
		{
			m_pStart		= _AllocateAndFill(n, val);	// 配置空间并设置初始值
			m_pEnd			= m_pStart + n;				// 调整结束位置
			m_pEndStorage	= m_pEnd;					// 调整结束位置
		}

		iterator _AllocateAndFill(size_type n, const T &val)
		{
			iterator res = AllocT::Allocate(n);
			
			try
			{
				__UninitializedFillN(res, n , val);
				return res;
			}
			catch(...)
			{
				// commit or rollback
			}

			return res;
		}

		template<typename ForwardIterator>
		iterator _AllocateAndCopy(size_type n, ForwardIterator pFirst, ForwardIterator pLast)
		{
			iterator res= AllocT::Allocate(n);

			try
			{
				__UninitializedCopy(pFirst, pLast, res);
				return res;
			}
			catch(...)
			{
				AllocT::Deallocate(res, n);
			}

			return res;
		}
	
		template<typename InputIterator>
		void _RangeInitialize(InputIterator pFirst, InputIterator pLast, InputIteratorTag)
		{
			for(; pFirst != pLast; ++pFirst)
			{
				push_back(*pFirst);
			}
		}
		
		template<typename InputIterator>
		void _RangeInsert(iterator pos, InputIterator pFirst, InputIterator pLast, InputIteratorTag)
		{
			for(; pFirst != pLast; ++pFirst)
			{
				pos = insert(pos, *pFirst);
				++pos;
			}
		}
	};


	template<typename T, typename AllocT>
	inline bool operator==(const Vector<T, AllocT> &lhs, const Vector<T, AllocT> &rhs)
	{
		return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template<typename T, typename AllocT>
	inline void swap(Vector<T, AllocT> &lhs, const Vector<T, AllocT> &rhs)
	{
		lhs.swap(rhs);
	}
}









#endif
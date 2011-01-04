#ifndef __VECTOR_HPP
#define __VECTOR_HPP

#include "../Basic.hpp"


namespace DataStructure
{
	template<typename T, typename AllocT = Allocator<T> >
	class Vector
	{
	public:
		// IteratorTraits<T>��������ͱ�
		typedef	T						value_type;
		typedef value_type*				pointer;
		typedef const value_type*		const_pointer;
		typedef value_type*				iterator;
		typedef const value_type*		const_iterator;	
		typedef value_type&				reference;
		typedef const value_type&		const_reference;
		typedef size_t					size_type;
		typedef ptrdiff_t				difference_type;
		// ����vector��ά������һ�������Կռ䣬���Բ���Ԫ���ͱ�ԭ��ָ�붼������Ϊ�������������Ҫ��

		typedef ReverseIterator<iterator>				ReverseIteratorType;
		typedef ReverseIterator<const_iterator>			ConstReverseIteratorType;
		


	private:
		// vector ���ü򵥵����������ռ䣬������������start��end�ֱ�ָ��ͷβ�����Ե�����end_of_storageָ������β��
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
					// ���Ŀ�������������
					iterator iterTemp = _AllocateAndCopy(rhs.end() - rhs.begin());

					__Destroy(m_pStart, m_pEnd);	// ��������Vector
					_Deallocate();					// �ͷſռ�

					m_pStart = iterTemp;
					m_pEndStorage = m_pStart + (rhs.end() - rhs.begin());
				}
				else if( size() >= rhs.size() )
				{
					// ���Ŀ��������СС�ڵ��ڱ���Ĵ�С
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


		// ȡ����һ��Ԫ��
		reference front()
		{
			return *begin();
		}
		const_reference front() const
		{
			return *begin();
		}
	
		// ȡ�����һ��Ԫ��
		reference back()
		{
			return *(end() - 1);
		}
		const_reference back() const
		{
			return *(end() - 1);
		}

		// ����һ��Ԫ�أ��ŵ����
		void push_back(const T &x)
		{
			if( m_pEnd != m_pEndStorage )
			{
				// ���б��ÿռ䣬ֱ���ڱ��ÿռ��й���Ԫ��
				__Construct(m_pEnd, x);

				++m_pEnd;	// ��������λ��
			}
			else
			{
				// ���ޱ��ÿռ�
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
				// �������ָ�����һ��Ԫ��
				// ��pos֮���Ԫ��һ��һ����ǰ�ƶ�
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
				// ���б��ÿռ�
				// �ڱ��ÿռ���ʼ������һ��Ԫ�أ�����Vector���һ��Ԫ��ֵΪ��ʼֵ
				__Construct(m_pEnd, *(m_pEnd - 1));
				++m_pEnd;
			}
			else
			{		// �џo���ÿ��g
				const size_type oldSize = size();
				const size_type len = oldSize != 0 ? 2 * oldSize : 1;
				// ��������ԭ�t�����ԭ��С��0���t���� 1����Ԫ�ش�С����
				// ���ԭ��С����0���t����ԭ��С�ăɱ���
				// ǰ����Á����ԭ�Y�ϣ����Μʂ��Á�������Y�ϡ�

				iterator newStart = AllocT::Allocate(len); // ���H����
				iterator newFinish = newStart;
				
				try
				{
					// ��ԭvector �ă��ݿ�ؐ���� vector��
					newFinish = __UninitializedCopy(m_pStart, pos, newStart);
					// ����Ԫ���O����ֵx
					__Construct(newFinish, x);
					// �{��ˮλ��
					++newFinish;
					// ��ԭvector �Ă��ÿ��g�еă���Ҳ�Ҍ���ؐ�^��ɶ��;����
					newFinish = __UninitializedCopy(pos, m_pEnd, newFinish);
				}
				catch(...) 
				{
					// "commit or rollback" �Z�⣺����ȫ���ɹ�����һ��������
					__Destroy(newStart, newFinish); 
					AllocT::Deallocate(newStart, len);
					throw;
				}

				// �☋�Kጷ�ԭ vector
				__Destroy(begin(), end());
				_Deallocate();

				// �{����������ָ����vector
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
			m_pStart		= _AllocateAndFill(n, val);	// ���ÿռ䲢���ó�ʼֵ
			m_pEnd			= m_pStart + n;				// ��������λ��
			m_pEndStorage	= m_pEnd;					// ��������λ��
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
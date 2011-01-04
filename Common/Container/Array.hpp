#ifndef __ARRAY_HPP
#define __ARRAY_HPP





namespace DataStructure
{

	template<typename T, size_t nSize>
	class Array
	{
	public:
		typedef T			value_type;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
		typedef T&			reference;
		typedef T*			iterator;
		typedef T*			pointer;
		typedef const T&	const_reference;
		typedef const T*	const_iterator;
		typedef const T*	const_pointer;

	private:
		value_type m_data[nSize];
		size_type m_nSize;

	public:
		Array()
			: m_nSize(nSize)
		{}

	private:
		Array(const Array &);
		Array &operator=(const Array &);

	public:
		void RangeCheck(size_type n)
		{
			if( n >= size() )
				throw range_error("Array<>: Index out of range");
		}

		void clear()
		{
			m_nSize = 0;
		}

		void resize(size_type n)
		{
			assert(n <= capacity());
			m_nSize = n;
		}

		size_type capacity() const
		{
			return nSize;
		}

		size_type size() const
		{
			return nSize;
		}

		bool empty() const
		{
			return m_nSize == 0;
		}

		bool full() const
		{
			return m_nSize >= nSize;
		}

		// direct access to data(read_only)
		const_pointer data() const
		{
			return m_data;
		}

		pointer c_array()
		{
			return m_data;
		}

		iterator begin() 
		{
			return m_data;
		}
		const_iterator begin() const
		{
			return m_data;
		}

		iterator end()
		{
			return m_data + m_nSize;
		}
		const_iterator end() const
		{
			return m_data + m_nSize;
		}
		
		reference at(size_type nIndex)
		{
			RangeCheck(nIndex);
			return m_data[nIndex];
		}
		const_reference at(size_type nIndex) const
		{
			RangeCheck(nIndex);
			return m_data[nIndex];
		}

		reference front()
		{
			return *begin();
		}
		const_reference front() const
		{
			return *begin();
		}

		reference back()
		{
			return *(end() - 1);
		}
		const_reference back() const
		{
			return *(end() - 1);
		}

		reference operator[](size_type nIndex)
		{
			assert(nIndex < size() && "out of range");
			return m_data[nIndex];
		}
		const_reference operator[](size_type nIndex) const
		{
			assert(nIndex < size() && "out of range");
			return m_data[nIndex];
		}

		void insert(iterator pos, const_reference val)
		{
			assert(pos >= begin() && pos <= end() );
			assert(!full());

			CopyBackward(pos, end(), end() + 1);
			*pos = val;
		}

		void erase(iterator pos)
		{
			assert(pos >= begin() && pos <= end() );
			copy(pos + 1, end(), pos);

			--m_nSize;
		}

		void push_back(const_reference val)
		{
			assert(!full());

			m_data[m_nSize++] = val;
		}

		void pop_back()
		{
			assert(!empty());

			--m_nSize;
		}
		
		void copy(const Array &rhs)
		{
			m_nSize = rhs.m_nSize;
			copy(rhs.m_data, rhs.m_data + m_nSize, m_data);
		}

	};



	// -------------------------------------------------------------------------
	// PriorityPushBack

	template<typename T>
	inline void PriorityPushBack(T &container, const typename T::value_type &val, const size_t nSize)
	{
		assert(container.capacity() >= nSize);

		if( container.size() >= nSize )
		{
			if( container.back() < val )
				return;

			container.pop_back();
		}

		const typename T::iterator iter = LowerBound(container.begin(), container.end(), val);
		container.insert(iter, val);
	}


	template<typename T, typename PredT>
	inline void PriorityPushBack(T &container, const typename T::value_type &val, const size_t nSize, PredT pred)
	{
		assert(container.capacity() >= nSize);

		if( container.size() >= nSize )
		{
			if( pred(container.back(), val) )
				return;

			container.pop_back();
		}

		const typename T::iterator iter = LowerBound(container.begin(), container.end(), val);
		container.insert(iter, val);
	}


	// -------------------------------------------------------------------------
	// class PriorityArray

	template<typename T, size_t nSize, typename Pred = Less<T> >
	class PriorityArray
		: public Array<T, nSize>
	{
	public:
		typedef Array<T, nSize>::const_reference const_reference;
	private:
		Pred m_pred;

	public:
		void push_back(const_reference val)
		{
			PriorityPushBack(*this, val, nSize, m_pred);
		}
	};
}




#endif
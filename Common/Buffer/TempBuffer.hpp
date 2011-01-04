#ifndef __TEMP_BUFFER_HPP
#define __TEMP_BUFFER_HPP

#include "../Basic.hpp"

namespace DataStructure
{
	template<typename T>
	inline Pair<T*, ptrdiff_t> GetTemporaryBuffer(ptrdiff_t len, T*) 
	{
		if( len > ptrdiff_t(INT_MAX / sizeof(T)) )
			len = INT_MAX / sizeof(T);

		while(len > 0)
		{
			T *tmp = Allocator<T>::Allocate(len);
			if( tmp != NULL )
				return Pair<T *, ptrdiff_t>(tmp, len);

			len /= 2;
		}

		return Pair<T*, ptrdiff_t>(NULL, NULL);
	}

	template<typename T>
	inline void ReleaseTemporaryBuffer(T* p) 
	{
		Allocator<T>::Deallocate(p);
	}


	// TemporaryBuffer
	template<typename ForwardIterator, typename T = IteratorTraits<ForwardIterator>::value_type, typename Alloc = Allocator<T> >
	class TemporaryBuffer
	{
	private:
		ptrdiff_t m_originalLen;
		ptrdiff_t m_len;
		T *m_pBuffer;

	public:
		TemporaryBuffer(ForwardIterator first, ForwardIterator last)
		{
			try
			{
				m_len = DistanceFunc(first, last);

				_AllocateBuffer();

				if( m_len > 0 )
				{
					_InitializeBuffer(*first, typename __TypeTraits<T>::has_trivial_default_constructor());
				}
			}
			catch(...)
			{
				Alloc::Deallocate(m_pBuffer, m_len); 
				m_pBuffer	= 0;
				m_len		= 0;
			}
		}

		~TemporaryBuffer() 
		{  
			__Destroy(m_pBuffer, m_pBuffer + m_len);
			Alloc::Deallocate(m_pBuffer, m_len); 
		}


	public:
		ptrdiff_t size() const 
		{ 
			return m_len; 
		}
		ptrdiff_t RequestedSize() const 
		{ 
			return m_originalLen;
		}

		T* begin() 
		{ 
			return m_pBuffer; 
		}
		T* end() 
		{ 
			return m_pBuffer + m_len; 
		}


	private:
		TemporaryBuffer(const TemporaryBuffer&);
		TemporaryBuffer &operator=(const TemporaryBuffer&);

		void _AllocateBuffer()
		{
			m_originalLen	= m_len;
			m_pBuffer		= NULL;

			if( m_len > INT_MAX / sizeof(T) )
				m_len = INT_MAX / sizeof(T);

			while( m_len > 0 ) 
			{
				m_pBuffer = Alloc::Allocate(m_len);

				if( m_pBuffer )
					break;

				m_len /= 2;
			}
		}

		void _InitializeBuffer(const T &, __TrueType) 
		{}
		void _InitializeBuffer(const T &val, __FalseType) 
		{
			__UninitializedFillN(m_pBuffer, m_len, val);
		}
	};

}

#endif
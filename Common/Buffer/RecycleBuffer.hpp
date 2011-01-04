#ifndef __RECYCLE_BUFFER_HPP
#define __RECYCLE_BUFFER_HPP

#include "../Basic.hpp"


namespace DataStructure
{

	// -------------------------------------------------------------------------
	// class RecycleBuffer
	
	template<typename T, typename AllocT = StdlibAlloc, int RoundSize = 64>
	class RecycleBuffer
	{
	public:
		
		typedef T				value_type;
		typedef T*				pointer;
		typedef const pointer	const_pointer;	
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef size_t			size_type;

		typedef pointer			iterator;
		typedef const_pointer	const_iterator;


	private:
		value_type *m_pBuf;
		size_type  m_nSize;


	public:
		RecycleBuffer()
			: m_nSize(RoundSize)
			, m_pBuf(NULL)
		{
			//m_pBuf = reinterpret_cast<T *>malloc(sizeof(T) * m_nSize);

			m_pBuf = reinterpret_cast<T *>(AllocT::Allocate(sizeof(T) * m_nSize));
		}
		~RecycleBuffer()
		{
			if( m_pBuf != NULL )
				//free(m_pBuf);
				AllocT::Deallocate(m_pBuf, m_nSize);
		}

	public:
		pointer data()
		{
			return m_pBuf;
		}
		const_pointer data() const
		{
			return m_pBuf;
		}

		iterator begin()
		{
			return m_pBuf;
		}
		const_iterator begin() const
		{
			return m_pBuf;
		}
	
		pointer Reverse(size_type nSize)
		{
			if( nSize > m_nSize )
			{
				AllocT::Deallocate(m_pBuf);
				m_nSize = Internal::Round(nSize, static_cast<size_type>(RoundSize));
				m_pBuf = reinterpret_cast<T *>(AllocT::Allocate(sizeof(T) * m_nSize));
			}

			return m_pBuf;
		}
	};
}


#endif
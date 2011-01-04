#ifndef __AUTO_BUFFER_HPP
#define __AUTO_BUFFER_HPP

#include "../Basic.hpp"


namespace DataStructure
{

	template<typename Alloc = Allocator<char> >
	class AutoBufferT
	{
	public:
		typedef typename Alloc::value_type		value_type;
		typedef typename Alloc::pointer			iterator;
		typedef typename Alloc::const_pointer	const_iterator;
		typedef typename Alloc::size_type		size_type;

	protected:
		value_type *m_data;
		size_type m_nSize;

	public:
		AutoBufferT()
			: m_data(NULL)
			, m_nSize(0)
		{}
		explicit AutoBufferT(size_type nSize)
			: m_data(NULL)
			, m_nSize(0)
		{	 
			Allocate(nSize);
		}

		~AutoBufferT()
		{
			if( m_data != NULL )
				Alloc::Deallocate(m_data, m_nSize);
		}


	public:
		value_type *Allocate(size_type nSize)
		{
			assert(m_data == NULL);

			m_nSize = nSize;
			m_data	= reinterpret_cast<value_type *>(Alloc::Allocate(nSize));

			return m_data;
		}

		void clear()
		{
			if( m_data != NULL )
			{
				Alloc::Deallocate(m_data);
				m_data = NULL;
			}
		}


		value_type *data()
		{
			return m_data;
		}

		const value_type *data() const
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


		bool Good() const
		{
			return m_data != NULL;
		}

		const size_type size() const 
		{
			return m_nSize;
		}
	};
}

#endif
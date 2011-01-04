#ifndef __ARCHIVE_MEMORY_ARCHIVE_HPP
#define __ARCHIVE_MEMORY_ARCHIVE_HPP



#include "Text.hpp"



namespace DataStructure
{
	
	//---------------------------------------------------
	// class MemWriteArchive

	// BaseStorageT = Vector or Deque or std::basic_string
	template<typename BaseStorageT>
	class MemWriteArchive
	{
	public:
		typedef	size_t												size_type;
		typedef size_type											pos_type;
		typedef ptrdiff_t											difference_type;

		typedef typename BaseStorageT::value_type					char_type;
		typedef typename ArchiveCharTraits<char_type>::uchar_type	uchar_type;
		typedef typename ArchiveCharTraits<char_type>::int_type		int_type;

		typedef typename BaseStorageT::iterator						iterator;
		typedef typename BaseStorageT::const_iterator				const_iterator;

	private:
		BaseStorageT	*m_stg;
		iterator		m_pos;

	public:
		MemWriteArchive()
			: m_stg(NULL)
		{}
		explicit MemWriteArchive(BaseStorageT &stg)
			: m_stg(&stg)
			, m_pos(stg.begin())
		{}


	private:
		MemWriteArchive(const MemWriteArchive &);
		MemWriteArchive &operator=(const MemWriteArchive &);

	public:
		void ClearChache()
		{

		}

		void Flush()
		{

		}


		iterator Tell() const
		{
			return m_pos - m_stg->begin();
		}

		void Seek(pos_type pos)
		{
			if( pos > size() )
				m_stg->resize(pos);

			m_pos = m_stg->begin() + pos;
		}

		void SeekEnd()
		{
			m_pos = m_stg->end();
		}


	public:
		void RePut(size_type offset, const char_type *lpBuf, size_type cch)
		{
			assert(offset > 0 && offset >= cch);

			std::copy(lpBuf, lpBuf + cch, m_pos - offset);
		}

		size_type Put(const char_type *lpBuf, size_type cch)
		{
			iterator PosEnd = m_stg->end();
			const size_type pos = PosEnd - m_pos;

			if( pos == 0 )
				m_stg->insert(PosEnd, lpBuf, lpBuf + cch);
			else if( pos < cch )
			{
				std::copy(lpBuf, lpBuf + pos, m_pos);
				m_stg->insert(PosEnd, lpBuf + pos, lpBuf + cch);
			}
			else
			{
				std::copy(lpBuf, lpBuf + cch, m_pos);
				m_pos += cch;

				return cch;
			}

			m_pos = m_stg->end();
			return cch;
		}

		size_type Put(char_type ch)
		{
			const iterator posEnd = m_stg->end();
			if( m_pos != posEnd )
				*m_pos++ = ch;
			else
			{
				m_stg->insert(posEnd, ch);
				m_pos = m_stg->end();
			}

			return 1;
		}

		size_type Put(const std::basic_string<char_type> &str)
		{
			return Put((char_type *)str.data(), str.length());
		}


	public:
		bool Open(BaseStorageT &stg)
		{
			if( Good() )
				return false;
			
			m_stg = &stg;
			m_pos = stg.begin();

			return true;
		}

		void Close()
		{
			m_stg = NULL;
		}

		pos_type size() const
		{
			assert(m_stg);
			return m_stg->size();
		}

		bool operator!() const
		{
			return m_stg == NULL;
		}

		bool Good() const
		{
			return m_stg != NULL;
		}

		bool Bad() const
		{
			return !Good();
		}
	};



	//---------------------------------------------------
	// class MemReadArchive

	template<typename IteratorT>
	class MemReadArchive
	{
	public:
		typedef typename ArchiveIteratorTraits<IteratorT>::char_type		char_type;
		typedef typename ArchiveCharTraits<char_type>::int_type				int_type;
		typedef typename ArchiveCharTraits<char_type>::uchar_type			uchar_type;

		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
		typedef size_type			pos_type;
		typedef difference_type		offset_type;
		typedef IteratorT			iterator;
		typedef IteratorT			const_iterator;

		enum { EndCh = ArchiveCharTraits<char_type>::EndCh };

		
	private:
		iterator m_pos;
		iterator m_first;
		iterator m_last;

	public:
		MemReadArchive(iterator first, iterator last)
			: m_pos(first)
			, m_first(first)
			, m_last(last)
		{}
		explicit MemReadArchive(iterator iter)
			: m_pos(iter)
			, m_first(iter)
			, m_last(End(iter))
		{}
		explicit MemReadArchive(char_type *first)
			: m_pos(first)
			, m_first(first)
			, m_last(End(iterator(first)))
		{}
		template<typename ContainerT>
		explicit MemReadArchive(const ContainerT &container)
			: m_pos(container.begin())
			, m_first(m_pos)
			, m_last(container.end())
		{}

	private:
		MemReadArchive(const MemReadArchive &);
		MemReadArchive &operator=(const MemReadArchive &);


	public:
		class Restriction
		{
		private:
			MemReadArchive &m_ar;
			iterator m_pos;
			iterator m_first;
			iterator m_last;

		public:
			Restriction(MemReadArchive &ar, iterator first, iterator last)
				: m_ar(ar)
				, m_pos(ar.m_pos)
				, m_first(ar.m_first)
				, m_last(ar.m_last)
			{
				ar.m_first = ar.m_pos = first;
				ar.m_last = last;
			}
			~Restriction()
			{
				m_ar.m_pos		= m_pos;
				m_ar.m_first	= m_first;
				m_ar.m_last		= m_last;
			}
		};

	public:
		void ClearCache()
		{}

		pos_type Tell() const
		{
			return m_pos - m_first; 
		}

		const_iterator Position() const
		{
			return m_pos;
		}

		void Seek(const_iterator pos)
		{
			m_pos = pos;
		}

		void Seek(pos_type offset)
		{
			if( offset > size() )
				throw std::out_of_range("Seek file position - out of range!");

			m_pos = m_first + offset;
		}

		void SeekEnd()
		{
			m_pos = m_last;
		}

		size_type Skip(size_type nMax)
		{
			const size_type nSize = m_last - m_pos;
			if( nSize < nMax )
			{
				m_pos = m_last;
				return nSize;
			}
			else
			{
				m_pos += nMax;
				return nMax;
			}
		}

		bool TrySkip(size_type nMax)
		{
			const size_type nSize = m_last - m_pos;
			if( nSize < nMax )
			{
				return false;
			}
			else
			{
				m_pos += nMax;
				return true;
			}
		}

		bool TryGet(char_type *lpBuf, size_type nMax)
		{
			const size_type nSize = m_last - m_pos;
			if( nSize < nMax )
				return false;
			else
			{
				std::copy(m_pos, m_pos + nMax, lpBuf);
				m_pos += nMax;

				return true;
			}
		}

		const char_type *Get(size_type nMax)
		{
			const size_type nSize = m_last - m_pos;
			if( nSize < nMax )
			{
				m_pos = m_last;
				return NULL;
			}
			else
			{
				const char_type *pData = &*m_pos;
				m_pos += nMax;

				return pData;
			}
		}

		size_type Get(char_type *lpBuf, size_type nMax)
		{
			const size_type nSize = m_last - m_pos;
			if( nSize < nMax )
			{
				std::copy(m_pos, m_last, lpBuf);
				m_pos = m_last;

				return nSize;
			}
			else
			{
				std::copy(m_pos, m_pos + nMax, lpBuf);
				m_pos += nMax;

				return nMax;
			}
		}

		size_type Get(char_type *pCh)
		{
			if( m_pos != m_last )
			{
				*pCh = *m_pos++;
				return 1;
			}
			else
				return 0;
		}

		int_type Get()
		{
			if( m_pos != m_last )
			{
				return *m_pos++;
			}
			else
				return EndCh;
		}


		void ReGet(size_type offset, char_type *lpBuf, size_type cch)
		{
			assert(offset >= 0 && offset >= cch);

			std::copy(m_pos - offset, m_pos - offset + cch, lpBuf);
		}

		int_type UnGet()
		{
			if( m_pos != m_first )
			{
				--m_pos;
				return 1;
			}
			else
				return 0;
		}

		void UnGet(int_type ch)
		{
			if( ch != EndCh )
				UnGet();
		}

		int_type Peek()
		{
			if( m_pos != m_last )
				return *m_pos;
			else
				return EndCh;
		}

		pos_type Size() const
		{
			return m_last - m_first;
		}

		bool operator!() const
		{
			return false;
		}

		bool Good() const
		{
			return true;
		}

		bool Bad() const
		{
			return false;
		}

	public:
		void SkipWS()
		{
			return IO::SkipWS(*this);
		}

		int_type GetNWS()
		{
			return IO::GetNWS(*this);
		}

		template<typename StringT>
		size_type GetLine(StringT &str)
		{
			return IO::GetLine(*this, str);
		}

		template<typename AllocT>
		size_type GetLine(AllocT &alloc, std::basic_string<char_type> &str)
		{
			return IO::GetLine(*this, alloc, str);
		}
	};
	
}




#endif
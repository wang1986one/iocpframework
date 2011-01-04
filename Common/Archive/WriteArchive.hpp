#ifndef __ARCHIVE_WRITE_ARCHIVE_HPP
#define __ARCHIVE_WRITE_ARCHIVE_HPP



#include "Basic.hpp"
#include "Text.hpp"
#include "Binary.hpp"


namespace DataStructure
{


	template<typename HandleT, typename StreamHandleT, typename CacheT = ArchiveCache>
	class WriteArchive
	{
	private:
		typedef typename CacheT::AllocatorType AllocatorType;

	public:
		enum { CacheSize = CacheT::CacheSize };
		enum { RoundSize = CacheT::RoundSize };


		typedef size_t									size_type;
		typedef ptrdiff_t								difference_type;

		typedef typename StreamHandleT::int_type		int_type;
		typedef typename StreamHandleT::char_type		char_type;
		typedef typename StreamHandleT::uchar_type		uchar_type;
		typedef typename StreamHandleT::pos_type		pos_type;
		typedef typename StreamHandleT::offset_type		offset_type;

	private:
		char_type*	m_pBufCur;
		char_type*	m_pBufStart;
		char_type*	m_pBufMax;
		size_type	m_nBufSize;

		StreamHandleT m_handle;
		AllocatorType m_alloc;


	public:
		explicit WriteArchive(AllocatorType alloc = AllocatorType())
			: m_alloc(alloc)
		{
			m_pBufStart	 = AllocArray<char_type>(alloc, CacheSize);
			m_nBufSize	 = CacheSize;
			m_pBufCur	 = m_pBufStart;
			m_pBufMax	 = m_pBufStart + m_nBufSize;
			// m_pBufCur - m_pBufStart为已经写入数据的缓存
		}

		explicit WriteArchive(HandleT hFile, AllocatorType alloc = AllocatorType())
			: m_alloc(alloc)
		{
			m_pBufStart	 = AllocArray<char_type>(alloc, CacheSize);
			m_nBufSize	 = CacheSize;
			m_pBufCur	 = m_pBufStart;
			m_pBufMax	 = m_pBufStart + m_nBufSize;

			m_handle.Open(hFile);
		}

		explicit WriteArchive(LPCSTR szFile, AllocatorType alloc = AllocatorType())
			: m_alloc(alloc)
		{
			m_pBufStart	 = AllocArray<char_type>(alloc, CacheSize);
			m_nBufSize	 = CacheSize;
			m_pBufCur	 = m_pBufStart;
			m_pBufMax	 = m_pBufStart + m_nBufSize;

			m_handle.OpenToWrite(szFile);
		}

		~WriteArchive()
		{
			if( m_pBufCur != m_pBufStart )
			{
				m_handle.Put(m_pBufStart, m_pBufCur - m_pBufStart);
			}

			m_alloc.Deallocate(m_pBufStart, m_nBufSize);
		}

	private:
		WriteArchive(const WriteArchive &);
		WriteArchive &operator=(const WriteArchive &);


	public:
		void ClearCache()
		{
			if( m_pBufCur != m_pBufStart )
			{
				char_type *pBufCur = m_pBufCur;
				m_pBufCur = m_pBufStart;

				m_handle.Put(m_pBufStart, pBufCur - m_pBufStart);
			}
		}

		void Flush()
		{
			ClearCache();
			m_handle.Flush();
		}

		pos_type Tell() const
		{
			return m_handle.Tell() + (m_pBufCur - m_pBufStart);
		}

		void Seek(pos_type offset)
		{
			ClearCache();
			m_handle.Seek(offset, SEEK_SET);
			m_pBufCur = m_pBufStart;
		}

		void SeekEnd()
		{
			ClearCache();
			m_handle.Seek(0, SEEK_END);
			m_pBufCur = m_pBufStart;
		}


	public:
		size_type Put(const char_type *const pBuf, size_type cch)
		{
			assert((pBuf != NULL || cch == 0) && m_pBufStart != NULL && m_pBufCur);
			
			// 1. 如果缓存足够
			const size_type nUnUsed = m_pBufMax - m_pBufCur;
			if( cch <= nUnUsed )
			{
				__CopyMemory(m_pBufCur, pBuf, cch);
				m_pBufCur += cch;
			}
			else
			{
				// 2.1 先将目标数据copy到缓存，直到填满为止
				__CopyMemory(m_pBufCur, pBuf, nUnUsed);

				// IO操作按磁盘的块边界对齐
				m_handle.Put(m_pBufStart, m_nBufSize);

				// 2.2 写满各整页
				const size_type nRes = cch - nUnUsed;
				const size_type nMod = nRes % m_nBufSize;
				const size_type nWritten = nRes - nMod;

				if( nWritten != 0 )
					m_handle.Put(pBuf + nUnUsed, nWritten);

				// 2.3 写最后一页
				__CopyMemory(m_pBufStart, pBuf + nUnUsed + nWritten, nMod);
				m_pBufCur = m_pBufStart + nMod;
			}
			
			return cch;
		}

		size_type Put(char_type ch)
		{
			if( m_pBufCur < m_pBufMax )
				*m_pBufCur++ = ch;
			else
			{
				// 写操作最好按磁盘的块边界对齐
				m_handle.Put(m_pBufStart, m_nBufSize);
				*m_pBufStart = ch;

				m_pBufCur = m_pBufStart + 1;
			}

			return 1;
		}

		size_type Put(const std::basic_string<char_type> &str)
		{
			return Put(str.data(), str.size());
		}

		void Reput(size_type offset, const char_type *pBuf, size_type cch)
		{
			assert(offset > 0 && offset >= cch);

			if( offset <= (m_pBufCur - m_pBufStart) )
				CopyMemory(m_pBufCur - offset, pBuf, cch);
			else
			{
				pos_type curPos = Tell();
				Seek(curPos - offset);

				Put(pBuf, cch);
				Seek(curPos);
			}
		}


	public:
		bool Open(LPCSTR szFile)
		{
			if( m_handle.Good() )
				return true;

			return m_handle.OpenToWrite(szFile);
		}
		void Open(HandleT hFile)
		{
			if( !m_handle.Good() )
				m_handle.Open(hFile);
		}

		bool OpenToAppend(LPCSTR szFile)
		{
			if( m_handle.Good() )
				return true;

			return m_handle.OpenToAppend(szFile);
		}

		void Close()
		{
			ClearCache();
			m_handle.Close();
		}

		void Attach(HandleT hFile)
		{
			ClearCache();
			m_handle.Attach(hFile);
		}

		HandleT Detach()
		{
			ClearCache();
			return m_handle.Detach();
		}


	public:
		bool Good() const
		{
			return m_handle.Good();
		}
		bool Bad() const
		{
			return m_handle.Bad();
		}

	public:
		template<typename DataIteratorT>
		WriteArchive &PrintLines(DataIteratorT first, DataIteratorT last)
		{
			IO::PrintLines(*this, first, last);
			return *this;
		}
		
		template<typename ContainerT>
		WriteArchive &PrintLines(const ContainerT &container)
		{
			IO::PrintLines(*this, container.begin(), container.end());
			return *this;
		}

		template<typename ContainerT, typename SeqT>
		WriteArchive &PrintLines(const ContainerT &container, const SeqT &seq)
		{
			IO::PrintLines(*this, container.begin(), container.end(), seq);
			return *this;
		}

		size_type Write(const void *pBuf, size_type size)
		{
			return Put(reinterpret_cast<const char_type *>(pBuf), size);
		}

		void Puts(const char_type *pBuf, size_type size)
		{
			IO::Puts(*this, pBuf, size);
		}

		void Puts(const std::string &str)
		{
			IO::Puts(*this, str);
		}

		void WPuts(const UINT16 *p, size_t cch)
		{
			IO::WPuts(*this, p, cch);
		}
		
	};

}




#endif
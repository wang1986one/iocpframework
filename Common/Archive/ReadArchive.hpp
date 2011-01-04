#ifndef __ARCHIVE_READ_ARCHIVE_HPP
#define __ARCHIVE_READ_ARCHIVE_HPP



#include "Basic.hpp"
#include "Text.hpp"
#include "Binary.hpp"


namespace DataStructure
{

	//------------------------------------------------
	// class ReadArchive

	template<typename HandleT, typename StreamHandleT, typename CacheT = ArchiveCache>
	class ReadArchive
	{
	private:
		typedef typename CacheT::AllocatorType AllocatorType;
	
	public:
		enum { EndCh = StreamHandleT::EndCh };
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
		explicit ReadArchive(AllocatorType alloc = AllocatorType())
			: m_alloc(alloc)
		{
			m_pBufStart = AllocArray<char_type>(m_alloc, CacheSize);
			m_nBufSize  = CacheSize;
			m_pBufCur	= m_pBufMax = m_pBufStart; 
		}
	
		explicit ReadArchive(HandleT hFile, pos_type pos = 0, AllocatorType alloc = AllocatorType() )
			: m_alloc(alloc)
		{
			m_pBufStart = AllocArray<char_type>(m_alloc, CacheSize);
			m_nBufSize  = CacheSize;
			m_pBufCur	= m_pBufMax = m_pBufStart; 

			m_handle.Open(hFile);
		}

		explicit ReadArchive(LPCSTR lpszFile, AllocatorType alloc = AllocatorType())
			: m_alloc(alloc)
		{
			m_pBufStart = AllocArray<char_type>(m_alloc, CacheSize);
			m_nBufSize  = CacheSize;
			m_pBufCur	= m_pBufMax = m_pBufStart; 

			m_handle.OpenToRead(lpszFile);
		}

		ReadArchive(const ReadArchive &rhs, bool bClone, AllocatorType alloc = AllocatorType())
			: m_alloc(alloc)
		{
			m_pBufStart = AllocArray<char_type>(m_alloc, CacheSize);
			m_nBufSize  = CacheSize;
			m_pBufCur	= m_pBufMax = m_pBufStart; 

			m_handle.Copy(rhs.m_handle);
		}

		~ReadArchive()
		{
			m_alloc.Deallocate(m_pBufStart, m_nBufSize);
		}

	private:
		ReadArchive(const ReadArchive &);
		ReadArchive &operator=(const ReadArchive &);


	public:
		void ClearCache()
		{
			m_pBufCur = m_pBufMax = m_pBufStart;
		}

		pos_type Tell() const
		{
			return m_handle.Tell() - (m_pBufMax - m_pBufCur);
		}

		void Seek(pos_type offset)
		{
			// 读状态：m_pBufMax - m_pBufStart不为m_nBufSize, 而是数据有效范围
			pos_type pos = m_handle.Tell();
			pos_type delta = pos - offset;

			if( delta <= static_cast<pos_type>(m_pBufMax - m_pBufStart) && delta > 0 )
			{
				m_pBufCur = m_pBufMax - ToUInt(delta);
			}
			else
			{
				m_handle.Seek(static_cast<offset_type>(offset), SEEK_SET);
				m_pBufCur = m_pBufMax = m_pBufStart;
			}
		}

		void SeekEnd()
		{
			m_handle.Seek(0, SEEK_END);
			m_pBufCur = m_pBufMax = m_pBufStart;
		}

		size_type Skip(size_type nMax)
		{
			// 在缓存数据足够时，读入数据并返回
			if( m_pBufCur + nMax <= m_pBufMax )
			{
				m_pBufCur += nMax;
			}
			// 在缓存数据不足时
			else
			{
				m_handle.Seek(nMax - (m_pBufMax - m_pBufCur), SEEK_CUR);
				m_pBufCur = m_pBufMax = m_pBufStart;
			}

			return nMax;
		}
	
		const char_type *Peek(size_type nMax)
		{
			if( m_pBufCur + nMax <= m_pBufMax )
				return m_pBufCur;
			else
				_DoPeek(nMax);
		}

		const char_type *Get(size_type nMax)
		{
			if( m_pBufCur + nMax <= m_pBufMax )
			{
				const char_type *p = m_pBufCur;
				m_pBufCur += nMax;

				return p;
			}
			else
			{
				const char_type *p = _DoPeek(nMax);
				if( p != NULL )
					m_pBufCur += nMax;
					
				return p;
			}
		}

		size_type Get(char_type *pBuf, size_type nMax)
		{
			assert(pBuf != NULL && m_pBufStart != NULL && m_pBufCur != NULL);

			// 1）在缓存数据足够时，读入数据并返回
			if( nMax + m_pBufCur <= m_pBufMax )
			{
				__CopyMemory(pBuf, m_pBufCur, nMax);
				m_pBufCur += nMax;

				return nMax;
			}

			// 2）在缓存数据不足时
			return _DoGet(pBuf, nMax);
		}

		size_type Get(char_type *ch)
		{
			if( m_pBufCur < m_pBufMax )
			{
				*ch = *m_pBufCur++;
				return 1;
			}
			else
			{
				return Get(ch, 1);
			}
		}

		int_type Get()
		{
			if( m_pBufCur < m_pBufMax )
				return *m_pBufCur++;

			char_type ch;
			return Get(&ch, 1) ? ch : EndCh;
		}

		void ReGet(size_type offset, char_type *pBuf, size_type cch)
		{
			assert( offset >= 0 && offset >= cch );

			if( offset <= m_pBufCur - m_pBufStart )		
				__CopyMemory(pBuf, m_pBufCur - offset, cch);
			else
			{
				pos_type curPos = Tell();
				Seek(curPos - offset);
				Get(pBuf, cch);
				Seek(curPos);
			}
		}

		int_type UnGet()
		{
			if( m_pBufCur <= m_pBufStart )
			{
				pos_type delta = m_nBufSize >> 2;
				pos_type curPos = Tell();

				if( curPos < delta )
					delta = curPos;

				m_handle.Seek(curPos - delta, SEEK_SET);
				m_pBufMax = m_pBufStart + m_handle.Get(m_pBufStart, m_nBufSize);
				m_pBufCur = m_pBufStart + ToUInt(delta);

				if( m_pBufCur <= m_pBufStart )
					return 0;
			}

			--m_pBufCur;
			return 1;
		}

		void UnGet(int_type ch)
		{
			if( ch != EndCh	)
				UnGet();
		}

		int_type Peek()
		{
			if( m_pBufCur < m_pBufMax )
				return *m_pBufCur;

			char_type ch = 0;
			if( !Get(&ch, 1) )
				return EndCh;

			--m_pBufCur;
			return ch;
		}

		
	public:
		bool Open(LPCSTR szFile)
		{
			if( m_handle.Good() )
				return true;

			return m_handle.OpenToRead(szFile);
		}

		void Open(HandleT hFile, pos_type pos = 0)
		{
			if( !m_handle.Good() )
				m_handle.Open(hFile);
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

		HandleT Dettach()
		{
			ClearCache();
			return m_handle.Dettach();
		}

	public:
		pos_type size() const
		{
			return m_handle.size();
		}

		bool operator!() const
		{
			return m_handle.Bad();
		}

		bool Good() const
		{
			return m_handle.Good();
		}

		bool Bad() const
		{
			return m_handle.Bad();
		}

	
	public:
		void SkipWS()
		{
			IO::SkipWS(*this);
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


	public:
		size_type Read(void *pBuf, size_type size)
		{
			return Get(reinterpret_cast<char_type *>(pBuf), size);
		}

		template<typename StringT>
		bool Gets(StringT &str)
		{
			return IO::Gets(*this, str);
		}

		template<typename AllocT>
		bool Gets(AllocT &alloc, std::basic_string<char_type> &str)
		{
			return IO::Gets(*this, alloc, str);
		}

	private:
		const char_type *_DoPeek(size_type nMax)
		{
			const size_type nRes = m_pBufMax - m_pBufCur;
			if( nMax > m_nBufSize )
			{
				const size_type nBufSize = Internal::Round(nMax, static_cast<size_type>(RoundSize));
				char_type *pNewBuf = AllocArray<char_type>(m_alloc, nBufSize);

				__CopyMemory(pNewBuf, m_pBufCur, nRes);
				m_alloc.Deallocate(m_pBufStart, m_nBufSize);
				m_pBufStart = pNewBuf;
				m_nBufSize	= nBufSize;
			}
			else
			{
				__CopyMemory(m_pBufStart, m_pBufCur, nRes);
			}

			const size_type nSize = m_handle.Get(m_pBufStart + nRes, m_nBufSize - nRes) + nRes;
			m_pBufCur = m_pBufStart;
			m_pBufMax = m_pBufStart + nSize;

			return nSize < nMax ? NULL : m_pBufStart;
		}


		size_type _DoGet(char_type *pBuf, size_type nMax)
		{
			// nRead	--- 读入的总字节数
			// nMax		--- 剩余字节数
			// nBlkRead --- 文件操作的返回值
			size_type nBlkRead = 0, nRead = 0;

			// a）先读入缓存中的数据
			nRead = m_pBufMax - m_pBufCur;
			__CopyMemory(pBuf, m_pBufCur, nRead);
			nMax -= nRead;
			m_pBufCur = m_pBufMax;

			// b）将m_nBufSize的整数倍内容读入
			size_type nTemp = nMax - nMax % m_nBufSize;
			if( nTemp != 0 )
			{
				nBlkRead = m_handle.Get(pBuf + nRead, nTemp);
				nRead += nBlkRead;

				if( nBlkRead < nTemp )
				{
					// 可能已经到了文件末端:
					return nRead;
				}

				nMax -= nTemp;
			}

			assert(nMax < m_nBufSize);

			// c）剩余的先读入到缓冲区中，再写入目标内存
			nBlkRead = m_handle.Get(m_pBufStart, m_nBufSize);
			m_pBufCur = m_pBufStart;
			m_pBufMax = m_pBufCur + nBlkRead;

			if( nBlkRead < nMax )
				nMax = nBlkRead;

			__CopyMemory(pBuf + nRead, m_pBufCur, nMax);
			m_pBufCur += nMax;
			nRead += nMax;

			return nRead;
		}
	};
}

#endif
#ifndef __ARCHIVE_POSIX_HPP
#define __ARCHIVE_POSIX_HPP




#include "WriteARchive.hpp"
#include "ReadArchive.hpp"



#pragma warning(disable:4996)	// for _open


namespace DataStructure
{
	//-----------------------------------------------------------
	// class PosixAdapter
	
	class PosixAdapter
	{
	public:
		enum { EndCh = -1 };
		enum { NullFD = -1 };

		typedef char			char_type;
		typedef unsigned char	uchar_type;
		typedef int				int_type;
		typedef size_t			size_type;
		typedef long			pos_type;
		typedef long			offset_type;

	private:
		typedef int				_Handle;

		enum { WriteMode = O_TRUNC | O_CREAT | O_WRONLY };
		enum { AppendMode = O_BINARY | O_RDWR | O_APPEND };
		enum { ReadMode = O_BINARY | O_RDONLY };
		enum { CMASK = 0644 };

	protected:
		_Handle m_fd;

	public:
		PosixAdapter()
			: m_fd(NullFD)
		{}
		~PosixAdapter()
		{
			if( m_fd != NullFD )
			{
				::_close(m_fd);
				m_fd = NullFD;
			}
		}


	public:
		void Attach(_Handle fd)
		{
			m_fd = fd;
		}
		_Handle Detach()
		{
			_Handle fd = m_fd;
			m_fd = NullFD;

			return fd;
		}

		void Open(_Handle fd)
		{
			assert(m_fd == NullFD);
			m_fd = fd;
		}

		bool OpenToWrite(LPCSTR szFile)
		{
			assert(m_fd == NullFD);
			
			m_fd = ::_open(szFile, WriteMode, CMASK);

			return Good() ? true : false;
		}
		bool OpenToAppend(LPCSTR szFile)
		{
			assert(m_fd == NullFD);
			m_fd = ::_open(szFile, AppendMode, CMASK);

			return Good() ? true : false;
		}
		bool OpenToRead(LPCSTR szFile)
		{
			assert(m_fd == NullFD);
			m_fd = ::_open(szFile, ReadMode, CMASK);

			return Good() ? true : false;
		}

		void Seek(const offset_type pos, int dir = SEEK_SET)
		{
			::_lseek(m_fd, pos, dir);
		}

		pos_type Tell() const
		{
			return ::_tell(m_fd);
		}

		pos_type Size() const
		{
			return ::_filelength(m_fd);
		}

		size_type PGet(char_type *pBuf, size_type cch, pos_type pos)
		{
			Seek(pos);

			return Get(pBuf, cch);
		}

		size_type Get(char_type *pBuf, size_type cch)
		{
			size_t n = ::_read(m_fd, pBuf, cch);
			return n > 0 ? n : 0;
		}

		size_type Put(const char_type *pBuf, size_type cch)
		{
			return ::_write(m_fd, pBuf, cch);
		}

		void Flush()
		{
			::_commit(m_fd);
		}

		void Close()
		{
			if( m_fd != NullFD )
			{
				::_close(m_fd);
				m_fd = NullFD;
			}
		}

		bool Good() const
		{
			return m_fd != NullFD;
		}
		bool Bad() const
		{
			return !Good();
		}


	public:
		operator _Handle() const
		{
			return m_fd;
		}

		bool operator!() const
		{
			return m_fd != NullFD;
		}
	};



	typedef WriteArchive<int, PosixAdapter>		PosixWriteArchive;
	typedef ReadArchive<int, PosixAdapter>		PosixReadeArchive;
}


#pragma warning(default:4996)


#endif
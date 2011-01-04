#ifndef __ARCHIVE_STDIO_HPP
#define __ARCHIVE_STDIO_HPP

#include "WriteArchive.hpp"
#include "ReadArchive.hpp"



namespace DataStructure
{

	//------------------------------------------------
	// class StdioAdpater

	class StdioAdpater
	{
	public:
		typedef char					char_type;
		typedef unsigned char			uchar_type;
		typedef int						int_type;
		typedef size_t					size_type;
		typedef unsigned long			pos_type;
		typedef long					offset_type;

		enum { EndCh = -1 };
		enum { NULL_FD = 0 };

	private:
		typedef FILE * _HANDLE;
		
	protected:
		_HANDLE m_pFile;
	
	public:
		StdioAdpater()
			: m_pFile(NULL)
		{}
		~StdioAdpater()
		{
			Close();
		}

	private:
		StdioAdpater(const StdioAdpater &);
		StdioAdpater &operator==(const StdioAdpater &);

	public:
		void Attach(_HANDLE pFile)
		{
			assert(m_pFile == NULL);
			m_pFile = pFile;
		}

		_HANDLE Detach()
		{
			_HANDLE pFile = m_pFile;
			m_pFile = NULL;

			return pFile;
		}

		void Open(_HANDLE pFile)
		{
			assert(m_pFile == NULL);
			assert(Tell() == 0);

			m_pFile = pFile;
		}

		bool OpenToWrite(LPCSTR szFile)
		{
			assert(m_pFile == NULL);

			::fopen_s(&m_pFile, szFile, "wb");

			return m_pFile != NULL ? true : false;
		}
		bool OpenToRead(LPCSTR szFile)
		{
			assert(m_pFile == NULL);

			::fopen_s(&m_pFile, szFile, "rb");

			return m_pFile != NULL ? true : false;
		}

		void Seek(const offset_type &pos, int dir = SEEK_SET)
		{
			::fseek(m_pFile, pos, dir);
		}

		pos_type Tell() const
		{
			return ::ftell(m_pFile);
		}

		pos_type Size() const
		{
			return _filelength(::_fileno(m_pFile));
		}

		size_type Get(char_type *pBuf, size_type cch)
		{
			assert(pBuf != NULL);
			return ::fread(pBuf, sizeof(char_type), cch, m_pFile);
		}

		size_type Put(const char_type *pBuf, size_type cch)
		{
			assert(pBuf != NULL);
			return ::fwrite(pBuf, sizeof(char_type), cch, m_pFile);
		}

		void Flush()
		{
			::fflush(m_pFile);
		}

		void Close()
		{
			if( m_pFile != NULL )
			{
				::fclose(m_pFile);
				m_pFile = NULL;
			}
		}


	public:
		operator _HANDLE() const
		{
			return m_pFile;
		}
	
		bool operator!() const
		{
			return m_pFile == NULL;
		}

		bool Good() const
		{
			return m_pFile != NULL;
		}

		bool Bad() const
		{
			return !Good();
		}
	};



	typedef WriteArchive<FILE *, StdioAdpater>	StdioWriteArchive;
	typedef ReadArchive<FILE *, StdioAdpater>	StdioReadArchive;
}





#endif
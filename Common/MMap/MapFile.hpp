#ifndef __MMAP_MAP_FILE_HPP
#define __MMAP_MAP_FILE_HPP


namespace DataStructure
{
	// -------------------------------------------------------------------------
	// struct MappingReadWrite

	struct MappingReadWrite
	{
		static const unsigned int PageProtect				= PAGE_READWRITE;
		static const unsigned int PageAccess				= FILE_MAP_READ | FILE_MAP_WRITE;
		static const unsigned int FileDesireAccess			= GENERIC_WRITE | GENERIC_READ;
		static const unsigned int FileShareMode				= FILE_SHARE_READ;
		static const unsigned int FileCreationDisposition	= OPEN_ALWAYS;
		static const unsigned int FileFlagsAndAttributes	= FILE_FLAG_WRITE_THROUGH | FILE_ATTRIBUTE_ARCHIVE;
		static const unsigned int GetSizeOnOpen				= FALSE;
	};

	// -------------------------------------------------------------------------
	// struct MappingReadOnly
	struct MappingReadOnly
	{
		static const unsigned int PageProtect				= PAGE_READONLY;
		static const unsigned int PageAccess				= FILE_MAP_READ;
		static const unsigned int FileDesireAccess			= GENERIC_READ;
		static const unsigned int FileShareMode				= FILE_SHARE_READ | FILE_SHARE_WRITE;
		static const unsigned int FileCreationDisposition	= OPEN_EXISTING;
		static const unsigned int FileFlagsAndAttributes	= FILE_FLAG_SEQUENTIAL_SCAN;
		static const unsigned int GetSizeOnOpen				= TRUE;
	};


	// -------------------------------------------------------------------------
	// class MapFile
	// 提供对File Mapping的直接操作

	template<typename Config>
	class MapFile
	{
	private:
		HANDLE m_hFile;
		HANDLE m_hFileMapping;


	public:
		typedef size_t	size_type;
		typedef UINT64	pos_type;

	public:
		MapFile()
			: m_hFile(INVALID_HANDLE_VALUE)
			, m_hFileMapping(NULL)
		{}
		template<typename T>
		explicit MapFile(T szFile, pos_type *offset = NULL)
			: m_hFile(INVALID_HANDLE_VALUE)
			, m_hFileMapping(NULL)
		{
			Open(szFile, offset);
		}

		~MapFile()
		{
			Close();
		}

	public:
		template<typename T>
		bool Open(T szFile, pos_type *offset = NULL)
		{
			if( Good() )
				return false;

			_CreateFileImpl(szFile);
			if( m_hFile == INVALID_HANDLE_VALUE )
				return false;

			if( Config::GetSizeOnOpen == TRUE )
			{
				DWORD dwFileSizeHigh = 0;
				DWORD dwFileSize = ::GetFileSize(m_hFile, &dwFileSizeHigh);

				m_hFileMapping = ::CreateFileMapping(m_hFile, NULL, 
					Config::PageProtect,
					dwFileSizeHigh,
					dwFileSize,
					NULL);

				if( offset != NULL )
					*offset = static_cast<pos_type>((dwFileSizeHigh << 32) | dwFileSize);
			}
			else
			{
				m_hFileMapping = NULL;
			}


			return true;
		}

		void Close()
		{
			if( m_hFile != INVALID_HANDLE_VALUE )
			{
				if( m_hFile != NULL )
				{
					::CloseHandle(m_hFileMapping);
					m_hFileMapping = NULL;
				}

				::CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;
			}
		}


		bool Good() const
		{
			return m_hFile != INVALID_HANDLE_VALUE;
		}


		bool Resize(pos_type newSize)
		{
			if( m_hFileMapping != NULL )
				::CloseHandle(m_hFileMapping);

			m_hFileMapping = ::CreateFileMapping(m_hFile, NULL,
				Config::PageProtect, 
				static_cast<DWORD>(newSize >> 32), 
				static_cast<DWORD>(newSize), 
				NULL);

			return m_hFileMapping ? true : false;
		}

		pos_type Size() const
		{
			DWORD dwFileSizeHigh = 0;
			DWORD dwFileSize = ::GetFileSize(m_hFile, &dwFileSizeHigh);

			return static_cast<UINT64>(dwFileSizeHigh << 32) | dwFileSize;
		}

		void *Map(pos_type offset, size_type size, void *pBaseAddr = NULL)
		{
			return ::MapViewOfFileEx(m_hFileMapping, 
				Config::PageAccess,
				static_cast<DWORD>(offset >> 32),
				static_cast<DWORD>(offset),
				size,
				pBaseAddr);
		}

		static void Unmap(void *p, size_type/* size*/)
		{
			::UnmapViewOfFile(p);
		}

		static bool Flush(void *p, size_type size)
		{
			return ::FlushViewOfFile(p, size) ? true : false;
		}


		private:
			void _CreateFileImpl(const char *szFile)
			{
				m_hFile = ::CreateFileA(szFile, 
					Config::FileDesireAccess, 
					Config::FileShareMode,
					NULL,
					Config::FileCreationDisposition,
					Config::FileFlagsAndAttributes,
					NULL);
			}

			void _CreateFileImpl(const wchar_t *szFile)
			{
				m_hFile = ::CreateFileW(szFile, 
					Config::FileDesireAccess, 
					Config::FileShareMode,
					NULL,
					Config::FileCreationDisposition,
					Config::FileFlagsAndAttributes,
					NULL);
			}
	};


	typedef MapFile<MappingReadOnly>	MapFileReadOnly;
	typedef MapFile<MappingReadWrite>	MapFileReadWrite;
}

#endif
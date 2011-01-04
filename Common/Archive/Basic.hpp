#ifndef __ARCHIVE_BASIC_HPP
#define __ARCHIVE_BASIC_HPP


#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>

#include "../Basic.hpp"
#include "../Memory.hpp"
#include "../Utility/CharType.hpp"



namespace DataStructure
{
	const int ARCHIVE_CACHE_SIZE = 4096;
	const int ARCHIVE_ROUND_SIZE = 4096;


	// -------------------------------------------------------------------------
	// class ArchiveCharTraits

	template<typename CharT>
	class ArchiveCharTraits;

	template<>
	class ArchiveCharTraits<char>
	{
	public:
		typedef char			char_type;
		typedef unsigned char	uchar_type;
		typedef int				int_type;

		enum { EndCh = -1 };
	};
	template<>
	class ArchiveCharTraits<wchar_t>
	{
	public:
		typedef wchar_t				char_type;
		typedef wchar_t				uchar_type;
		typedef int					int_type;

		enum { EndCh = -1 };
	};


	// -------------------------------------------------------------------------
	// class ArchiveIteratorTraits

	template<typename IteratorT>
	class ArchiveIteratorTraits
	{
	public:
		typedef typename IteratorT::value_type	value_type;
		typedef value_type						char_type;
	};

	template<>
	class ArchiveIteratorTraits<char *>
	{
	public:
		typedef char		value_type;
		typedef value_type	char_type;
	};
	template<>
	class ArchiveIteratorTraits<const char *>
	{
	public:
		typedef char		value_type;
		typedef value_type	char_type;
	};
	template<>
	class ArchiveIteratorTraits<wchar_t *>
	{
	public:
		typedef wchar_t		value_type;
		typedef value_type	char_type;
	};
	template<>
	class ArchiveIteratorTraits<const wchar_t *>
	{
	public:
		typedef wchar_t		value_type;
		typedef value_type	char_type;
	};



	// -------------------------------------------------------------------------
	// ToInt, ToUInt

	inline int ToInt(int li)			
	{ return li; }

	inline unsigned int ToUInt(unsigned int li)	
	{ return li; }



	// -------------------------------------------------------------------------
	// __CopyMemory
	
	inline void __CopyMemory(char *dest, const char *src, size_t n)
	{
		::memcpy(dest, src, n);
	}
	inline void __CopyMemory(wchar_t *dest, const wchar_t *src, size_t n)
	{
		::wmemcpy(dest, src, n);
	}



	// -------------------------------------------------------------------------
	// IsStgWritable
	
	inline bool IsStgWritable(IStorage* pstg)
	{
		STATSTG statstg;
		pstg->Stat(&statstg, STATFLAG_NONAME);
		return (statstg.grfMode & (STGM_READWRITE|STGM_WRITE)) != 0;
	}

	inline bool IsStgWritable(IStream* pstm)
	{
		STATSTG statstg;
		pstm->Stat(&statstg, STATFLAG_NONAME);
		return (statstg.grfMode & (STGM_READWRITE|STGM_WRITE)) != 0;
	}


	// -------------------------------------------------------------------------
	// class IoException

	class IOException
		: public std::exception
	{
	private:
		HRESULT m_scode;

	public:
		IOException(HRESULT sc, const char *pErrMsg = "IO Exception")
			: exception(pErrMsg)
			, m_scode(sc)
		{}

	public:
		HRESULT GetError() const
		{
			return m_scode;
		}
	};


	// -------------------------------------------------------------------------
	// class ArchiveCache

	class ArchiveCache
	{
	public:
		typedef StdlibAlloc AllocatorType;

		enum { CacheSize = ARCHIVE_CACHE_SIZE };
		enum { RoundSize = ARCHIVE_ROUND_SIZE };
	};


	// -------------------------------------------------------------------------
	// PutN
	
	/*template<typename OStreamT, typename CharT>
	inline void PutN(OStreamT &os, size_t nSize, const CharT &val)
	{
		for(size_t i = 0; i < nSize; i++)
			os.Put(val);
	}*/

	template<typename OStreamT, typename CharT>
	inline void PutN(OStreamT &os, size_t nSize, const CharT &val)
	{
		os.Put(nSize, val); 
	}



	//-----------------------------------------------------
	// Resize

	template<typename ContainerT>
	inline typename ContainerT::value_type* Resize(ContainerT &container, size_t size)
	{
		container.resize(size);
		return &*container.begin();
	}



	//------------------------------------------------------
	// End

	template<typename IteratorT>
	inline IteratorT End(IteratorT first)
	{
		while(first != NULL)
			++first;

		return ++first;
	}
}



#endif